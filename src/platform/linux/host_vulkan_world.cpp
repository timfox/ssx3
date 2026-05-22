#include "platform/host_vulkan_world.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace host {
namespace {

constexpr VkDeviceSize kVertexStride = 9 * sizeof(float);

struct MeshUniformGpu {
    float mvp[16];
};

[[noreturn]] void throw_world(const char* what) {
    throw std::runtime_error(what);
}

std::vector<char> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file) {
        throw_world(("failed to open file: " + path).c_str());
    }
    const auto size = file.tellg();
    std::vector<char> buffer(static_cast<size_t>(size));
    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

VkFormat find_depth_format(VkPhysicalDevice physical_device) {
    const VkFormat candidates[] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT};
    for (const VkFormat format : candidates) {
        VkFormatProperties props{};
        vkGetPhysicalDeviceFormatProperties(physical_device, format, &props);
        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            return format;
        }
    }
    return VK_FORMAT_D32_SFLOAT;
}

} // namespace

bool VulkanWorldGpu::init(VkPhysicalDevice physical_device,
                          VkDevice device,
                          VkQueue graphics_queue,
                          uint32_t graphics_family,
                          VkCommandPool command_pool,
                          VkFormat swap_format,
                          const std::vector<VkImageView>& swap_views,
                          VkExtent2D extent,
                          const std::string& shader_dir,
                          const WorldScene& scene) {
    physical_device_ = physical_device;
    device_ = device;
    graphics_queue_ = graphics_queue;
    graphics_family_ = graphics_family;
    command_pool_ = command_pool;
    swap_format_ = swap_format;
    shader_dir_ = shader_dir;
    extent_ = extent;
    depth_format_ = find_depth_format(physical_device);

    try {
        if (!create_render_pass()) {
            return false;
        }
        if (!create_depth_resources()) {
            return false;
        }
        if (!create_framebuffers(swap_views)) {
            return false;
        }
        if (!create_pipeline()) {
            return false;
        }
        if (!upload_mesh(scene.terrain,
                         vertex_buffer_,
                         vertex_memory_,
                         index_buffer_,
                         index_memory_,
                         index_count_)) {
            return false;
        }
        marker_index_count_ = 0;
        if (!scene.object_markers.indices.empty()) {
            if (!upload_mesh(scene.object_markers,
                             marker_vertex_buffer_,
                             marker_vertex_memory_,
                             marker_index_buffer_,
                             marker_index_memory_,
                             marker_index_count_)) {
                return false;
            }
        }
        set_camera(scene.camera, static_cast<float>(extent.width) / static_cast<float>(extent.height));
    } catch (const std::exception& ex) {
        std::cerr << "[world]   Vulkan world init failed: " << ex.what() << '\n';
        shutdown();
        return false;
    }

    std::cout << "[world]   3D terrain ready (" << extent.width << "x" << extent.height << ", "
              << index_count_ / 3 << " terrain tris, " << marker_index_count_ / 3 << " object tris)\n";
    return true;
}

void VulkanWorldGpu::shutdown() {
    if (device_ == VK_NULL_HANDLE) {
        return;
    }
    vkDeviceWaitIdle(device_);
    destroy_mesh();
    destroy_pipeline();
    destroy_framebuffers();
    destroy_depth();
    if (render_pass_ != VK_NULL_HANDLE) {
        vkDestroyRenderPass(device_, render_pass_, nullptr);
        render_pass_ = VK_NULL_HANDLE;
    }
    device_ = VK_NULL_HANDLE;
}

void VulkanWorldGpu::on_swapchain_changed(const std::vector<VkImageView>& swap_views, VkExtent2D extent) {
    if (device_ == VK_NULL_HANDLE) {
        return;
    }
    vkDeviceWaitIdle(device_);
    destroy_framebuffers();
    destroy_depth();
    extent_ = extent;
    create_depth_resources();
    create_framebuffers(swap_views);
}

void VulkanWorldGpu::set_camera(const WorldCamera& camera, float aspect) {
    if (!uniform_mapped_) {
        return;
    }
    const Mat4 mvp = world_camera_mvp(camera, aspect);
    MeshUniformGpu ubo{};
    std::memcpy(ubo.mvp, mvp.m, sizeof(ubo.mvp));
    std::memcpy(uniform_mapped_, &ubo, sizeof(ubo));
}

void VulkanWorldGpu::record(VkCommandBuffer cmd, std::uint32_t swap_image_index, VkExtent2D extent) {
    if (pipeline_ == VK_NULL_HANDLE || index_count_ == 0 ||
        swap_image_index >= framebuffers_.size()) {
        return;
    }

    VkClearValue clears[2]{};
    clears[0].color = {{0.42f, 0.55f, 0.78f, 1.0f}};
    clears[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo rp{};
    rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp.renderPass = render_pass_;
    rp.framebuffer = framebuffers_[swap_image_index];
    rp.renderArea.offset = {0, 0};
    rp.renderArea.extent = extent;
    rp.clearValueCount = 2;
    rp.pClearValues = clears;

    vkCmdBeginRenderPass(cmd, &rp, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, extent};
    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
    const VkBuffer vbs[] = {vertex_buffer_};
    const VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmd, 0, 1, vbs, offsets);
    vkCmdBindIndexBuffer(cmd, index_buffer_, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout_, 0, 1, &descriptor_set_, 0,
                              nullptr);
    vkCmdDrawIndexed(cmd, index_count_, 1, 0, 0, 0);

    if (marker_index_count_ > 0) {
        const VkBuffer marker_vbs[] = {marker_vertex_buffer_};
        const VkDeviceSize marker_offsets[] = {0};
        vkCmdBindVertexBuffers(cmd, 0, 1, marker_vbs, marker_offsets);
        vkCmdBindIndexBuffer(cmd, marker_index_buffer_, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmd, marker_index_count_, 1, 0, 0, 0);
    }

    vkCmdEndRenderPass(cmd);
}

bool VulkanWorldGpu::create_render_pass() {
    VkAttachmentDescription attachments[2]{};
    attachments[0].format = swap_format_;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    attachments[1].format = depth_format_;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_ref{};
    color_ref.attachment = 0;
    color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkAttachmentReference depth_ref{};
    depth_ref.attachment = 1;
    depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_ref;
    subpass.pDepthStencilAttachment = &depth_ref;

    VkSubpassDependency dep{};
    dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass = 0;
    dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                       VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                       VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.srcAccessMask = 0;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 2;
    info.pAttachments = attachments;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dep;

    return vkCreateRenderPass(device_, &info, nullptr, &render_pass_) == VK_SUCCESS;
}

bool VulkanWorldGpu::create_depth_resources() {
    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent = {extent_.width, extent_.height, 1};
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = depth_format_;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    if (vkCreateImage(device_, &image_info, nullptr, &depth_image_) != VK_SUCCESS) {
        return false;
    }

    VkMemoryRequirements mem_req{};
    vkGetImageMemoryRequirements(device_, depth_image_, &mem_req);
    VkMemoryAllocateInfo alloc{};
    alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc.allocationSize = mem_req.size;
    alloc.memoryTypeIndex = find_memory_type(mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(device_, &alloc, nullptr, &depth_memory_) != VK_SUCCESS) {
        return false;
    }
    vkBindImageMemory(device_, depth_image_, depth_memory_, 0);

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = depth_image_;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = depth_format_;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.layerCount = 1;
    return vkCreateImageView(device_, &view_info, nullptr, &depth_view_) == VK_SUCCESS;
}

bool VulkanWorldGpu::create_framebuffers(const std::vector<VkImageView>& swap_views) {
    framebuffers_.resize(swap_views.size());
    for (std::size_t i = 0; i < swap_views.size(); ++i) {
        VkImageView attachments[] = {swap_views[i], depth_view_};
        VkFramebufferCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = render_pass_;
        info.attachmentCount = 2;
        info.pAttachments = attachments;
        info.width = extent_.width;
        info.height = extent_.height;
        info.layers = 1;
        if (vkCreateFramebuffer(device_, &info, nullptr, &framebuffers_[i]) != VK_SUCCESS) {
            return false;
        }
    }
    return true;
}

bool VulkanWorldGpu::create_pipeline() {
    const auto vert_code = read_file(shader_dir_ + "/mesh.vert.spv");
    const auto frag_code = read_file(shader_dir_ + "/mesh.frag.spv");
    const VkShaderModule vert_mod = create_shader_module(vert_code);
    const VkShaderModule frag_mod = create_shader_module(frag_code);

    VkPipelineShaderStageCreateInfo stages[2]{};
    stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = vert_mod;
    stages[0].pName = "main";
    stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = frag_mod;
    stages[1].pName = "main";

    VkVertexInputBindingDescription binding{};
    binding.binding = 0;
    binding.stride = kVertexStride;
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attrs[3]{};
    attrs[0] = {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0};
    attrs[1] = {1, 0, VK_FORMAT_R32G32B32_SFLOAT, 3 * sizeof(float)};
    attrs[2] = {2, 0, VK_FORMAT_R32G32B32_SFLOAT, 6 * sizeof(float)};

    VkPipelineVertexInputStateCreateInfo vertex_input{};
    vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input.vertexBindingDescriptionCount = 1;
    vertex_input.pVertexBindingDescriptions = &binding;
    vertex_input.vertexAttributeDescriptionCount = 3;
    vertex_input.pVertexAttributeDescriptions = attrs;

    VkPipelineInputAssemblyStateCreateInfo input_asm{};
    input_asm.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_asm.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewport_state{};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo raster{};
    raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster.polygonMode = VK_POLYGON_MODE_FILL;
    raster.cullMode = VK_CULL_MODE_BACK_BIT;
    raster.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    raster.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo msaa{};
    msaa.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    msaa.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depth_state{};
    depth_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_state.depthTestEnable = VK_TRUE;
    depth_state.depthWriteEnable = VK_TRUE;
    depth_state.depthCompareOp = VK_COMPARE_OP_LESS;

    VkPipelineColorBlendAttachmentState blend_att{};
    blend_att.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                               VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo blend{};
    blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blend.attachmentCount = 1;
    blend.pAttachments = &blend_att;

    VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamic{};
    dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic.dynamicStateCount = 2;
    dynamic.pDynamicStates = dynamic_states;

    VkDescriptorSetLayoutBinding ubo_binding{};
    ubo_binding.binding = 0;
    ubo_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    ubo_binding.descriptorCount = 1;
    ubo_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings = &ubo_binding;
    if (vkCreateDescriptorSetLayout(device_, &layout_info, nullptr, &descriptor_layout_) != VK_SUCCESS) {
        vkDestroyShaderModule(device_, frag_mod, nullptr);
        vkDestroyShaderModule(device_, vert_mod, nullptr);
        return false;
    }

    VkPipelineLayoutCreateInfo pipe_layout{};
    pipe_layout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipe_layout.setLayoutCount = 1;
    pipe_layout.pSetLayouts = &descriptor_layout_;
    if (vkCreatePipelineLayout(device_, &pipe_layout, nullptr, &pipeline_layout_) != VK_SUCCESS) {
        vkDestroyShaderModule(device_, frag_mod, nullptr);
        vkDestroyShaderModule(device_, vert_mod, nullptr);
        return false;
    }

    VkGraphicsPipelineCreateInfo pipe_info{};
    pipe_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipe_info.stageCount = 2;
    pipe_info.pStages = stages;
    pipe_info.pVertexInputState = &vertex_input;
    pipe_info.pInputAssemblyState = &input_asm;
    pipe_info.pViewportState = &viewport_state;
    pipe_info.pRasterizationState = &raster;
    pipe_info.pMultisampleState = &msaa;
    pipe_info.pDepthStencilState = &depth_state;
    pipe_info.pColorBlendState = &blend;
    pipe_info.pDynamicState = &dynamic;
    pipe_info.layout = pipeline_layout_;
    pipe_info.renderPass = render_pass_;
    pipe_info.subpass = 0;

    const bool ok = vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &pipe_info, nullptr, &pipeline_) ==
                    VK_SUCCESS;
    vkDestroyShaderModule(device_, frag_mod, nullptr);
    vkDestroyShaderModule(device_, vert_mod, nullptr);
    if (!ok) {
        return false;
    }

    create_buffer(sizeof(MeshUniformGpu),
                  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  uniform_buffer_,
                  uniform_memory_);
    vkMapMemory(device_, uniform_memory_, 0, sizeof(MeshUniformGpu), 0, &uniform_mapped_);

    VkDescriptorPoolSize pool_size{};
    pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size.descriptorCount = 1;
    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.maxSets = 1;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &pool_size;
    if (vkCreateDescriptorPool(device_, &pool_info, nullptr, &descriptor_pool_) != VK_SUCCESS) {
        return false;
    }

    VkDescriptorSetAllocateInfo set_alloc{};
    set_alloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    set_alloc.descriptorPool = descriptor_pool_;
    set_alloc.descriptorSetCount = 1;
    set_alloc.pSetLayouts = &descriptor_layout_;
    if (vkAllocateDescriptorSets(device_, &set_alloc, &descriptor_set_) != VK_SUCCESS) {
        return false;
    }

    VkDescriptorBufferInfo buf_info{};
    buf_info.buffer = uniform_buffer_;
    buf_info.range = sizeof(MeshUniformGpu);
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = descriptor_set_;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.pBufferInfo = &buf_info;
    vkUpdateDescriptorSets(device_, 1, &write, 0, nullptr);
    return true;
}

bool VulkanWorldGpu::upload_mesh(const WorldMesh& mesh,
                                 VkBuffer& vertex_buffer,
                                 VkDeviceMemory& vertex_memory,
                                 VkBuffer& index_buffer,
                                 VkDeviceMemory& index_memory,
                                 std::uint32_t& index_count) {
    std::vector<float> packed;
    packed.reserve(mesh.vertices.size() * 9);
    for (const WorldVertex& vtx : mesh.vertices) {
        packed.push_back(vtx.px);
        packed.push_back(vtx.py);
        packed.push_back(vtx.pz);
        packed.push_back(vtx.nx);
        packed.push_back(vtx.ny);
        packed.push_back(vtx.nz);
        packed.push_back(vtx.r);
        packed.push_back(vtx.g);
        packed.push_back(vtx.b);
    }

    const VkDeviceSize vb_size = static_cast<VkDeviceSize>(packed.size() * sizeof(float));
    const VkDeviceSize ib_size = static_cast<VkDeviceSize>(mesh.indices.size() * sizeof(std::uint32_t));
    index_count = static_cast<std::uint32_t>(mesh.indices.size());

    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory staging_mem = VK_NULL_HANDLE;
    create_buffer(vb_size + ib_size,
                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  staging,
                  staging_mem);

    void* mapped = nullptr;
    vkMapMemory(device_, staging_mem, 0, vb_size + ib_size, 0, &mapped);
    std::memcpy(mapped, packed.data(), static_cast<std::size_t>(vb_size));
    std::memcpy(static_cast<char*>(mapped) + vb_size, mesh.indices.data(), static_cast<std::size_t>(ib_size));
    vkUnmapMemory(device_, staging_mem);

    create_buffer(vb_size,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                  vertex_buffer,
                  vertex_memory);
    create_buffer(ib_size,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                  index_buffer,
                  index_memory);

    VkCommandBuffer cmd = VK_NULL_HANDLE;
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool_;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;
    vkAllocateCommandBuffers(device_, &alloc_info, &cmd);

    VkCommandBufferBeginInfo begin{};
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &begin);

    VkBufferCopy copies[2]{};
    copies[0].size = vb_size;
    copies[1].srcOffset = vb_size;
    copies[1].dstOffset = 0;
    copies[1].size = ib_size;
    vkCmdCopyBuffer(cmd, staging, vertex_buffer, 1, &copies[0]);
    vkCmdCopyBuffer(cmd, staging, index_buffer, 1, &copies[1]);
    vkEndCommandBuffer(cmd);

    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmd;
    vkQueueSubmit(graphics_queue_, 1, &submit, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphics_queue_);
    vkFreeCommandBuffers(device_, command_pool_, 1, &cmd);
    vkDestroyBuffer(device_, staging, nullptr);
    vkFreeMemory(device_, staging_mem, nullptr);
    return true;
}

void VulkanWorldGpu::destroy_framebuffers() {
    for (auto fb : framebuffers_) {
        vkDestroyFramebuffer(device_, fb, nullptr);
    }
    framebuffers_.clear();
}

void VulkanWorldGpu::destroy_depth() {
    if (depth_view_ != VK_NULL_HANDLE) {
        vkDestroyImageView(device_, depth_view_, nullptr);
        depth_view_ = VK_NULL_HANDLE;
    }
    if (depth_image_ != VK_NULL_HANDLE) {
        vkDestroyImage(device_, depth_image_, nullptr);
        depth_image_ = VK_NULL_HANDLE;
    }
    if (depth_memory_ != VK_NULL_HANDLE) {
        vkFreeMemory(device_, depth_memory_, nullptr);
        depth_memory_ = VK_NULL_HANDLE;
    }
}

void VulkanWorldGpu::destroy_pipeline() {
    if (uniform_mapped_) {
        vkUnmapMemory(device_, uniform_memory_);
        uniform_mapped_ = nullptr;
    }
    if (uniform_buffer_ != VK_NULL_HANDLE) {
        vkDestroyBuffer(device_, uniform_buffer_, nullptr);
        uniform_buffer_ = VK_NULL_HANDLE;
    }
    if (uniform_memory_ != VK_NULL_HANDLE) {
        vkFreeMemory(device_, uniform_memory_, nullptr);
        uniform_memory_ = VK_NULL_HANDLE;
    }
    if (descriptor_pool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device_, descriptor_pool_, nullptr);
        descriptor_pool_ = VK_NULL_HANDLE;
    }
    descriptor_set_ = VK_NULL_HANDLE;
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

void VulkanWorldGpu::destroy_mesh_buffers(VkBuffer& vertex_buffer,
                                          VkDeviceMemory& vertex_memory,
                                          VkBuffer& index_buffer,
                                          VkDeviceMemory& index_memory,
                                          std::uint32_t& index_count) {
    if (vertex_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device_, vertex_buffer, nullptr);
        vertex_buffer = VK_NULL_HANDLE;
    }
    if (vertex_memory != VK_NULL_HANDLE) {
        vkFreeMemory(device_, vertex_memory, nullptr);
        vertex_memory = VK_NULL_HANDLE;
    }
    if (index_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device_, index_buffer, nullptr);
        index_buffer = VK_NULL_HANDLE;
    }
    if (index_memory != VK_NULL_HANDLE) {
        vkFreeMemory(device_, index_memory, nullptr);
        index_memory = VK_NULL_HANDLE;
    }
    index_count = 0;
}

void VulkanWorldGpu::destroy_mesh() {
    destroy_mesh_buffers(vertex_buffer_, vertex_memory_, index_buffer_, index_memory_, index_count_);
    destroy_mesh_buffers(marker_vertex_buffer_,
                         marker_vertex_memory_,
                         marker_index_buffer_,
                         marker_index_memory_,
                         marker_index_count_);
}

VkShaderModule VulkanWorldGpu::create_shader_module(const std::vector<char>& code) const {
    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule mod = VK_NULL_HANDLE;
    if (vkCreateShaderModule(device_, &info, nullptr, &mod) != VK_SUCCESS) {
        throw_world("failed to create shader module");
    }
    return mod;
}

uint32_t VulkanWorldGpu::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties mem{};
    vkGetPhysicalDeviceMemoryProperties(physical_device_, &mem);
    for (uint32_t i = 0; i < mem.memoryTypeCount; ++i) {
        if ((type_filter & (1u << i)) && (mem.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw_world("failed to find memory type");
    return 0;
}

void VulkanWorldGpu::create_buffer(VkDeviceSize size,
                                   VkBufferUsageFlags usage,
                                   VkMemoryPropertyFlags properties,
                                   VkBuffer& buffer,
                                   VkDeviceMemory& memory) const {
    VkBufferCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.size = size;
    info.usage = usage;
    if (vkCreateBuffer(device_, &info, nullptr, &buffer) != VK_SUCCESS) {
        throw_world("failed to create buffer");
    }
    VkMemoryRequirements req{};
    vkGetBufferMemoryRequirements(device_, buffer, &req);
    VkMemoryAllocateInfo alloc{};
    alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc.allocationSize = req.size;
    alloc.memoryTypeIndex = find_memory_type(req.memoryTypeBits, properties);
    if (vkAllocateMemory(device_, &alloc, nullptr, &memory) != VK_SUCCESS) {
        throw_world("failed to allocate buffer memory");
    }
    vkBindBufferMemory(device_, buffer, memory, 0);
}

} // namespace host
