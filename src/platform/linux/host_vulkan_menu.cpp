#include "platform/host_vulkan_menu.h"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace host {
namespace {

constexpr int kMaxTextures = 8;
constexpr VkDeviceSize kVertexStride = 8 * sizeof(float); // pos.xy, uv.xy, color.rgba

[[noreturn]] void throw_menu(const char* what) {
    throw std::runtime_error(what);
}

std::vector<char> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file) {
        throw_menu(("failed to open file: " + path).c_str());
    }
    const auto size = file.tellg();
    std::vector<char> buffer(static_cast<size_t>(size));
    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

} // namespace

bool VulkanMenuGpu::init(VkPhysicalDevice physical_device,
                         VkDevice device,
                         VkQueue graphics_queue,
                         uint32_t graphics_family,
                         VkRenderPass render_pass,
                         VkFormat swap_format,
                         const std::string& shader_dir) {
    physical_device_ = physical_device;
    device_ = device;
    graphics_queue_ = graphics_queue;
    graphics_family_ = graphics_family;
    render_pass_ = render_pass;
    swap_format_ = swap_format;
    shader_dir_ = shader_dir;
    textures_.resize(kMaxTextures);
    return create_pipeline();
}

void VulkanMenuGpu::shutdown() {
    if (device_ == VK_NULL_HANDLE) {
        return;
    }
    vkDeviceWaitIdle(device_);
    destroy_pipeline();
    for (auto& tex : textures_) {
        if (tex.view != VK_NULL_HANDLE) {
            vkDestroyImageView(device_, tex.view, nullptr);
            tex.view = VK_NULL_HANDLE;
        }
        if (tex.image != VK_NULL_HANDLE) {
            vkDestroyImage(device_, tex.image, nullptr);
            tex.image = VK_NULL_HANDLE;
        }
        if (tex.memory != VK_NULL_HANDLE) {
            vkFreeMemory(device_, tex.memory, nullptr);
            tex.memory = VK_NULL_HANDLE;
        }
    }
    textures_.clear();
    device_ = VK_NULL_HANDLE;
}

bool VulkanMenuGpu::load_assets(const std::string& asset_dir) {
    const std::string logo = asset_dir + "/title_logo.png";
    const std::string logo_fallback = asset_dir + "/ui_ssxt.png";
    const std::string mountain = asset_dir + "/title_mountain.png";
    const std::string mountain_fallback = asset_dir + "/ui_moun.png";
    if (!load_png_texture(logo, 0) && !load_png_texture(logo_fallback, 0)) {
        std::cerr << "[menu]    missing title logo (run scripts/extract_menu_assets.py)\n";
        return false;
    }
    if (!load_png_texture(mountain, 1) && !load_png_texture(mountain_fallback, 1)) {
        std::cerr << "[menu]    missing title mountain\n";
        return false;
    }
    if (!create_snow_texture()) {
        return false;
    }
    return true;
}

void VulkanMenuGpu::on_swapchain_resize(VkExtent2D extent) {
    extent_ = extent;
}

void VulkanMenuGpu::on_render_pass_changed(VkRenderPass render_pass) {
    render_pass_ = render_pass;
    destroy_pipeline();
    create_pipeline();
}

void VulkanMenuGpu::record(VkCommandBuffer cmd, VkExtent2D draw_extent, const MenuFrame& frame) {
    if (pipeline_ == VK_NULL_HANDLE || textures_.empty()) {
        return;
    }

    std::vector<float> verts;
    verts.reserve(frame.sprites.size() * 6 * 8);
    for (const MenuSprite& sprite : frame.sprites) {
        append_sprite_quad(sprite, draw_extent, verts);
    }
    if (verts.empty()) {
        return;
    }

    const VkDeviceSize needed = static_cast<VkDeviceSize>(verts.size() * sizeof(float));
    if (needed > vertex_buffer_capacity_) {
        if (vertex_buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(device_, vertex_buffer_, nullptr);
            vkFreeMemory(device_, vertex_buffer_memory_, nullptr);
            vertex_buffer_ = VK_NULL_HANDLE;
            vertex_buffer_memory_ = VK_NULL_HANDLE;
        }
        vertex_buffer_capacity_ = needed * 2;
        create_buffer(vertex_buffer_capacity_,
                      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      vertex_buffer_,
                      vertex_buffer_memory_);
    }

    void* mapped = nullptr;
    vkMapMemory(device_, vertex_buffer_memory_, 0, needed, 0, &mapped);
    std::memcpy(mapped, verts.data(), static_cast<size_t>(needed));
    vkUnmapMemory(device_, vertex_buffer_memory_);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);

    VkViewport viewport{};
    viewport.width = static_cast<float>(draw_extent.width);
    viewport.height = static_cast<float>(draw_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{};
    scissor.extent = draw_extent;
    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(cmd, 0, 1, &vertex_buffer_, &offset);

    uint32_t first_vertex = 0;
    for (size_t i = 0; i < frame.sprites.size(); ++i) {
        const int tex = frame.sprites[i].texture_index;
        if (tex < 0 || tex >= static_cast<int>(descriptor_sets_.size())) {
            first_vertex += 6;
            continue;
        }
        vkCmdBindDescriptorSets(cmd,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline_layout_,
                                0,
                                1,
                                &descriptor_sets_[static_cast<size_t>(tex)],
                                0,
                                nullptr);
        vkCmdDraw(cmd, 6, 1, first_vertex, 0);
        first_vertex += 6;
    }
}

void VulkanMenuGpu::screen_to_ndc(float sx,
                                  float sy,
                                  float sw,
                                  float sh,
                                  VkExtent2D extent,
                                  float out[8]) const {
    const float fb_w = static_cast<float>(std::max<uint32_t>(extent.width, 1u));
    const float fb_h = static_cast<float>(std::max<uint32_t>(extent.height, 1u));
    const float x0 = (sx / fb_w) * 2.0f - 1.0f;
    const float x1 = ((sx + sw) / fb_w) * 2.0f - 1.0f;
    const float y0 = 1.0f - (sy / fb_h) * 2.0f;
    const float y1 = 1.0f - ((sy + sh) / fb_h) * 2.0f;
    out[0] = x0;
    out[1] = y1;
    out[2] = x1;
    out[3] = y1;
    out[4] = x1;
    out[5] = y0;
    out[6] = x0;
    out[7] = y0;
}

void VulkanMenuGpu::append_sprite_quad(const MenuSprite& sprite,
                                       VkExtent2D extent,
                                       std::vector<float>& verts) const {
    float ndc[8];
    screen_to_ndc(sprite.x, sprite.y, sprite.w, sprite.h, extent, ndc);

    const float u_corners[4] = {sprite.u0, sprite.u1, sprite.u1, sprite.u0};
    const float v_corners[4] = {sprite.v1, sprite.v1, sprite.v0, sprite.v0};
    const int tri[] = {0, 1, 2, 0, 2, 3};
    for (int i = 0; i < 6; ++i) {
        const int corner = tri[i];
        const bool top = corner == 0 || corner == 1;
        verts.push_back(ndc[corner * 2 + 0]);
        verts.push_back(ndc[corner * 2 + 1]);
        verts.push_back(u_corners[corner]);
        verts.push_back(v_corners[corner]);
        if (sprite.gradient_y) {
            verts.push_back(top ? sprite.r : sprite.r2);
            verts.push_back(top ? sprite.g : sprite.g2);
            verts.push_back(top ? sprite.b : sprite.b2);
            verts.push_back(top ? sprite.a : sprite.a2);
        } else {
            verts.push_back(sprite.r);
            verts.push_back(sprite.g);
            verts.push_back(sprite.b);
            verts.push_back(sprite.a);
        }
    }
}

uint32_t VulkanMenuGpu::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties mem_properties{};
    vkGetPhysicalDeviceMemoryProperties(physical_device_, &mem_properties);
    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i) {
        if ((type_filter & (1u << i)) &&
            (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw_menu("failed to find suitable memory type");
    return 0;
}

void VulkanMenuGpu::create_buffer(VkDeviceSize size,
                                  VkBufferUsageFlags usage,
                                  VkMemoryPropertyFlags properties,
                                  VkBuffer& buffer,
                                  VkDeviceMemory& memory) const {
    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(device_, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
        throw_menu("failed to create buffer");
    }

    VkMemoryRequirements mem_requirements{};
    vkGetBufferMemoryRequirements(device_, buffer, &mem_requirements);
    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties);
    if (vkAllocateMemory(device_, &alloc_info, nullptr, &memory) != VK_SUCCESS) {
        throw_menu("failed to allocate buffer memory");
    }
    vkBindBufferMemory(device_, buffer, memory, 0);
}

VkShaderModule VulkanMenuGpu::create_shader_module(const std::vector<char>& code) const {
    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule module = VK_NULL_HANDLE;
    if (vkCreateShaderModule(device_, &create_info, nullptr, &module) != VK_SUCCESS) {
        throw_menu("failed to create shader module");
    }
    return module;
}

bool VulkanMenuGpu::create_pipeline() {
    const std::string vert_path = shader_dir_ + "/sprite.vert.spv";
    const std::string frag_path = shader_dir_ + "/sprite.frag.spv";
    auto vert_code = read_file(vert_path);
    auto frag_code = read_file(frag_path);
    VkShaderModule vert_module = create_shader_module(vert_code);
    VkShaderModule frag_module = create_shader_module(frag_code);

    VkDescriptorSetLayoutBinding sampler_binding{};
    sampler_binding.binding = 0;
    sampler_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler_binding.descriptorCount = 1;
    sampler_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings = &sampler_binding;
    if (vkCreateDescriptorSetLayout(device_, &layout_info, nullptr, &descriptor_layout_) != VK_SUCCESS) {
        return false;
    }

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = &descriptor_layout_;
    if (vkCreatePipelineLayout(device_, &pipeline_layout_info, nullptr, &pipeline_layout_) != VK_SUCCESS) {
        return false;
    }

    struct Vertex {
        float pos[2];
        float uv[2];
        float color[4];
    };

    VkVertexInputBindingDescription binding{};
    binding.binding = 0;
    binding.stride = sizeof(Vertex);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attrs[3]{};
    attrs[0] = {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, pos)};
    attrs[1] = {1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)};
    attrs[2] = {2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, color)};

    VkPipelineVertexInputStateCreateInfo vertex_input{};
    vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input.vertexBindingDescriptionCount = 1;
    vertex_input.pVertexBindingDescriptions = &binding;
    vertex_input.vertexAttributeDescriptionCount = 3;
    vertex_input.pVertexAttributeDescriptions = attrs;

    VkPipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewport_state{};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState blend_attachment{};
    blend_attachment.blendEnable = VK_TRUE;
    blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
    blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo color_blending{};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &blend_attachment;

    VkPipelineShaderStageCreateInfo stages[2]{};
    stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = vert_module;
    stages[0].pName = "main";
    stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = frag_module;
    stages[1].pName = "main";

    VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamic_state{};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = 2;
    dynamic_state.pDynamicStates = dynamic_states;

    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = stages;
    pipeline_info.pVertexInputState = &vertex_input;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = &dynamic_state;
    pipeline_info.layout = pipeline_layout_;
    pipeline_info.renderPass = render_pass_;
    pipeline_info.subpass = 0;

    const VkResult result =
        vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline_);
    vkDestroyShaderModule(device_, frag_module, nullptr);
    vkDestroyShaderModule(device_, vert_module, nullptr);
    return result == VK_SUCCESS;
}

void VulkanMenuGpu::destroy_pipeline() {
    if (vertex_buffer_ != VK_NULL_HANDLE) {
        vkDestroyBuffer(device_, vertex_buffer_, nullptr);
        vertex_buffer_ = VK_NULL_HANDLE;
    }
    if (vertex_buffer_memory_ != VK_NULL_HANDLE) {
        vkFreeMemory(device_, vertex_buffer_memory_, nullptr);
        vertex_buffer_memory_ = VK_NULL_HANDLE;
    }
    vertex_buffer_capacity_ = 0;

    if (descriptor_pool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device_, descriptor_pool_, nullptr);
        descriptor_pool_ = VK_NULL_HANDLE;
    }
    descriptor_sets_.clear();

    if (pipeline_ != VK_NULL_HANDLE) {
        vkDestroyPipeline(device_, pipeline_, nullptr);
        pipeline_ = VK_NULL_HANDLE;
    }
    if (pipeline_layout_ != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
        pipeline_layout_ = VK_NULL_HANDLE;
    }
    if (descriptor_layout_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(device_, descriptor_layout_, nullptr);
        descriptor_layout_ = VK_NULL_HANDLE;
    }
}

bool VulkanMenuGpu::load_png_texture(const std::string& path, int index) {
    int w = 0;
    int h = 0;
    int comp = 0;
    stbi_uc* pixels = stbi_load(path.c_str(), &w, &h, &comp, 4);
    if (!pixels) {
        return false;
    }
    const bool ok = load_rgba_texture(pixels, w, h, index);
    stbi_image_free(pixels);
    return ok;
}

bool VulkanMenuGpu::load_rgba_texture(const unsigned char* pixels, int width, int height, int index) {
    if (!pixels || index < 0 || index >= kMaxTextures || width <= 0 || height <= 0) {
        return false;
    }
    const int w = width;
    const int h = height;

    GpuTexture& tex = textures_[static_cast<size_t>(index)];
    if (tex.view) {
        vkDestroyImageView(device_, tex.view, nullptr);
        tex.view = VK_NULL_HANDLE;
    }
    if (tex.image) {
        vkDestroyImage(device_, tex.image, nullptr);
        tex.image = VK_NULL_HANDLE;
    }
    if (tex.memory) {
        vkFreeMemory(device_, tex.memory, nullptr);
        tex.memory = VK_NULL_HANDLE;
    }

    tex.width = w;
    tex.height = h;

    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent = {static_cast<uint32_t>(w), static_cast<uint32_t>(h), 1};
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    if (vkCreateImage(device_, &image_info, nullptr, &tex.image) != VK_SUCCESS) {
        return false;
    }

    VkMemoryRequirements mem_req{};
    vkGetImageMemoryRequirements(device_, tex.image, &mem_req);
    VkMemoryAllocateInfo alloc{};
    alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc.allocationSize = mem_req.size;
    alloc.memoryTypeIndex = find_memory_type(mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(device_, &alloc, nullptr, &tex.memory) != VK_SUCCESS) {
        return false;
    }
    vkBindImageMemory(device_, tex.image, tex.memory, 0);

    const VkDeviceSize image_size = static_cast<VkDeviceSize>(w) * h * 4;
    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    create_buffer(image_size,
                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  staging_buffer,
                  staging_memory);
    void* mapped = nullptr;
    vkMapMemory(device_, staging_memory, 0, image_size, 0, &mapped);
    std::memcpy(mapped, pixels, static_cast<size_t>(image_size));
    vkUnmapMemory(device_, staging_memory);

    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = graphics_family_;
    pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    vkCreateCommandPool(device_, &pool_info, nullptr, &pool);

    VkCommandBuffer cmd = VK_NULL_HANDLE;
    VkCommandBufferAllocateInfo cmd_alloc{};
    cmd_alloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd_alloc.commandPool = pool;
    cmd_alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmd_alloc.commandBufferCount = 1;
    vkAllocateCommandBuffers(device_, &cmd_alloc, &cmd);

    VkCommandBufferBeginInfo begin{};
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &begin);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = tex.image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.layerCount = 1;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    vkCmdPipelineBarrier(cmd,
                         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0,
                         0,
                         nullptr,
                         0,
                         nullptr,
                         1,
                         &barrier);

    VkBufferImageCopy region{};
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.layerCount = 1;
    region.imageExtent = {static_cast<uint32_t>(w), static_cast<uint32_t>(h), 1};
    vkCmdCopyBufferToImage(cmd, staging_buffer, tex.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(cmd,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                         0,
                         0,
                         nullptr,
                         0,
                         nullptr,
                         1,
                         &barrier);

    vkEndCommandBuffer(cmd);
    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmd;
    vkQueueSubmit(graphics_queue_, 1, &submit, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphics_queue_);

    vkFreeCommandBuffers(device_, pool, 1, &cmd);
    vkDestroyCommandPool(device_, pool, nullptr);
    vkDestroyBuffer(device_, staging_buffer, nullptr);
    vkFreeMemory(device_, staging_memory, nullptr);

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = tex.image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = VK_FORMAT_R8G8B8A8_UNORM;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.layerCount = 1;
    if (vkCreateImageView(device_, &view_info, nullptr, &tex.view) != VK_SUCCESS) {
        return false;
    }

    if (descriptor_pool_ == VK_NULL_HANDLE) {
        VkDescriptorPoolSize pool_size{};
        pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_size.descriptorCount = kMaxTextures;
        VkDescriptorPoolCreateInfo pool_create{};
        pool_create.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_create.maxSets = kMaxTextures;
        pool_create.poolSizeCount = 1;
        pool_create.pPoolSizes = &pool_size;
        if (vkCreateDescriptorPool(device_, &pool_create, nullptr, &descriptor_pool_) != VK_SUCCESS) {
            return false;
        }
    }

    while (static_cast<int>(descriptor_sets_.size()) <= index) {
        VkDescriptorSet set = VK_NULL_HANDLE;
        VkDescriptorSetAllocateInfo set_alloc{};
        set_alloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        set_alloc.descriptorPool = descriptor_pool_;
        set_alloc.descriptorSetCount = 1;
        set_alloc.pSetLayouts = &descriptor_layout_;
        vkAllocateDescriptorSets(device_, &set_alloc, &set);
        descriptor_sets_.push_back(set);
    }

    VkSampler sampler = VK_NULL_HANDLE;
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    vkCreateSampler(device_, &sampler_info, nullptr, &sampler);

    VkDescriptorImageInfo image_info_desc{};
    image_info_desc.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    image_info_desc.imageView = tex.view;
    image_info_desc.sampler = sampler;

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = descriptor_sets_[static_cast<size_t>(index)];
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.pImageInfo = &image_info_desc;
    vkUpdateDescriptorSets(device_, 1, &write, 0, nullptr);
    vkDestroySampler(device_, sampler, nullptr);

    return true;
}

bool VulkanMenuGpu::create_snow_texture() {
    constexpr int kSize = 32;
    std::vector<unsigned char> pixels(static_cast<size_t>(kSize * kSize * 4));
    const float center = (kSize - 1) * 0.5f;
    const float radius = center;
    for (int y = 0; y < kSize; ++y) {
        for (int x = 0; x < kSize; ++x) {
            const float dx = static_cast<float>(x) - center;
            const float dy = static_cast<float>(y) - center;
            const float dist = std::sqrt(dx * dx + dy * dy) / radius;
            const float alpha = std::clamp(1.0f - dist, 0.0f, 1.0f);
            const size_t i = static_cast<size_t>((y * kSize + x) * 4);
            pixels[i + 0] = 255;
            pixels[i + 1] = 255;
            pixels[i + 2] = 255;
            pixels[i + 3] = static_cast<unsigned char>(alpha * alpha * 255.0f);
        }
    }

    return load_rgba_texture(pixels.data(), kSize, kSize, 2);
}

} // namespace host
