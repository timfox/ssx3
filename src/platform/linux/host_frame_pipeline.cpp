#include "platform/host_upscale.h"
#include "platform/host_dlss.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace host {
namespace {

std::vector<char> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file) {
        throw std::runtime_error("failed to open shader: " + path);
    }
    const auto size = file.tellg();
    std::vector<char> buffer(static_cast<size_t>(size));
    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

VkExtent2D scaled_extent(VkExtent2D output, float scale) {
    VkExtent2D extent{};
    extent.width = std::max(1u, static_cast<uint32_t>(std::lround(output.width * scale)));
    extent.height = std::max(1u, static_cast<uint32_t>(std::lround(output.height * scale)));
    return extent;
}

} // namespace

bool HostFramePipeline::init(VkInstance instance,
                             const VulkanDeviceContext& ctx,
                             const char* shader_dir,
                             VkRenderPass shared_render_pass,
                             UpscaleSettings settings,
                             VkExtent2D output_extent) {
    instance_ = instance;
    ctx_ = ctx;
    settings_ = settings;
    settings_.shader_dir = shader_dir;
    render_pass_ = shared_render_pass;
    output_extent_ = output_extent;
    render_extent_ = scaled_extent(output_extent, settings.render_scale);

    if (settings.backend == UpscaleBackend::Direct) {
        active_ = false;
        backend_in_use_ = UpscaleBackend::Direct;
        return true;
    }

    if (!create_offscreen_target()) {
        return false;
    }

    if (settings.backend == UpscaleBackend::DLSS && init_dlss()) {
        backend_in_use_ = UpscaleBackend::DLSS;
        active_ = true;
        std::cout << "[upscale] DLSS backend (" << render_extent_.width << "x" << render_extent_.height
                  << " -> " << output_extent_.width << "x" << output_extent_.height << ")\n";
        return true;
    }

    if (settings.backend == UpscaleBackend::DLSS) {
        std::cout << "[upscale] DLSS unavailable, falling back to compute\n";
    }

    if (!init_compute(shader_dir)) {
        destroy_offscreen_target();
        return false;
    }

    backend_in_use_ = UpscaleBackend::Compute;
    active_ = true;
    std::cout << "[upscale] compute backend (" << render_extent_.width << "x" << render_extent_.height
              << " -> " << output_extent_.width << "x" << output_extent_.height << ")\n";
    return true;
}

void HostFramePipeline::shutdown() {
    shutdown_dlss();
    shutdown_compute();
    destroy_offscreen_target();
    active_ = false;
    backend_in_use_ = UpscaleBackend::Direct;
}

void HostFramePipeline::resize(VkExtent2D output_extent) {
    output_extent_ = output_extent;
    render_extent_ = scaled_extent(output_extent_, settings_.render_scale);
    if (!active_) {
        return;
    }

    vkDeviceWaitIdle(ctx_.device);
    const UpscaleBackend backend = backend_in_use_;
    shutdown_dlss();
    shutdown_compute();
    destroy_offscreen_target();

    if (!create_offscreen_target()) {
        active_ = false;
        return;
    }

    if (backend == UpscaleBackend::DLSS) {
        if (!init_dlss()) {
            backend_in_use_ = UpscaleBackend::Compute;
            init_compute(settings_.shader_dir);
        }
    } else {
        init_compute(settings_.shader_dir);
    }
}

const char* HostFramePipeline::active_backend_name() const {
    switch (backend_in_use_) {
    case UpscaleBackend::Compute:
        return "compute";
    case UpscaleBackend::DLSS:
        return "dlss";
    default:
        return "direct";
    }
}

bool HostFramePipeline::create_offscreen_target() {
    try {
        create_image(ctx_.physical_device,
                     ctx_.device,
                     render_extent_.width,
                     render_extent_.height,
                     ctx_.swapchain_format,
                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                         VK_IMAGE_USAGE_STORAGE_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                     offscreen_image_,
                     offscreen_memory_);
        create_image_view(ctx_.device, offscreen_image_, ctx_.swapchain_format, VK_IMAGE_ASPECT_COLOR_BIT,
                          offscreen_view_);

        VkFramebufferCreateInfo fb_info{};
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.renderPass = render_pass_;
        fb_info.attachmentCount = 1;
        fb_info.pAttachments = &offscreen_view_;
        fb_info.width = render_extent_.width;
        fb_info.height = render_extent_.height;
        fb_info.layers = 1;
        if (vkCreateFramebuffer(ctx_.device, &fb_info, nullptr, &offscreen_framebuffer_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create offscreen framebuffer");
        }

        create_image(ctx_.physical_device,
                     ctx_.device,
                     output_extent_.width,
                     output_extent_.height,
                     ctx_.swapchain_format,
                     VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                     resolve_image_,
                     resolve_memory_);
        create_image_view(ctx_.device, resolve_image_, ctx_.swapchain_format, VK_IMAGE_ASPECT_COLOR_BIT,
                          resolve_view_);
        return true;
    } catch (const std::exception& ex) {
        std::cerr << "[upscale] offscreen target failed: " << ex.what() << '\n';
        destroy_offscreen_target();
        return false;
    }
}

void HostFramePipeline::destroy_offscreen_target() {
    if (offscreen_framebuffer_ != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(ctx_.device, offscreen_framebuffer_, nullptr);
        offscreen_framebuffer_ = VK_NULL_HANDLE;
    }
    if (offscreen_view_ != VK_NULL_HANDLE) {
        vkDestroyImageView(ctx_.device, offscreen_view_, nullptr);
        offscreen_view_ = VK_NULL_HANDLE;
    }
    if (offscreen_image_ != VK_NULL_HANDLE) {
        vkDestroyImage(ctx_.device, offscreen_image_, nullptr);
        offscreen_image_ = VK_NULL_HANDLE;
    }
    if (offscreen_memory_ != VK_NULL_HANDLE) {
        vkFreeMemory(ctx_.device, offscreen_memory_, nullptr);
        offscreen_memory_ = VK_NULL_HANDLE;
    }
    if (resolve_view_ != VK_NULL_HANDLE) {
        vkDestroyImageView(ctx_.device, resolve_view_, nullptr);
        resolve_view_ = VK_NULL_HANDLE;
    }
    if (resolve_image_ != VK_NULL_HANDLE) {
        vkDestroyImage(ctx_.device, resolve_image_, nullptr);
        resolve_image_ = VK_NULL_HANDLE;
    }
    if (resolve_memory_ != VK_NULL_HANDLE) {
        vkFreeMemory(ctx_.device, resolve_memory_, nullptr);
        resolve_memory_ = VK_NULL_HANDLE;
    }
}

bool HostFramePipeline::init_compute(const char* shader_dir) {
    try {
        const std::string spirv_path = std::string(shader_dir) + "/upscale.comp.spv";
        auto code = read_file(spirv_path);

        VkSamplerCreateInfo sampler_info{};
        sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler_info.magFilter = VK_FILTER_LINEAR;
        sampler_info.minFilter = VK_FILTER_LINEAR;
        sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        if (vkCreateSampler(ctx_.device, &sampler_info, nullptr, &sampler_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create sampler");
        }

        VkDescriptorSetLayoutBinding bindings[2]{};
        bindings[0].binding = 0;
        bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        bindings[1].binding = 1;
        bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bindings[1].descriptorCount = 1;
        bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo layout_info{};
        layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layout_info.bindingCount = 2;
        layout_info.pBindings = bindings;
        if (vkCreateDescriptorSetLayout(ctx_.device, &layout_info, nullptr, &descriptor_layout_) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor layout");
        }

        VkPushConstantRange push_range{};
        push_range.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        push_range.offset = 0;
        push_range.size = sizeof(uint32_t) * 4 + sizeof(float);

        VkDescriptorPoolSize pool_sizes[2]{};
        pool_sizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[0].descriptorCount = 1;
        pool_sizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        pool_sizes[1].descriptorCount = 1;

        VkDescriptorPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.maxSets = 1;
        pool_info.poolSizeCount = 2;
        pool_info.pPoolSizes = pool_sizes;
        if (vkCreateDescriptorPool(ctx_.device, &pool_info, nullptr, &descriptor_pool_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool");
        }

        VkDescriptorSetAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = descriptor_pool_;
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &descriptor_layout_;
        if (vkAllocateDescriptorSets(ctx_.device, &alloc_info, &descriptor_set_) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor set");
        }

        VkShaderModuleCreateInfo module_info{};
        module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        module_info.codeSize = code.size();
        module_info.pCode = reinterpret_cast<const uint32_t*>(code.data());
        VkShaderModule module = VK_NULL_HANDLE;
        if (vkCreateShaderModule(ctx_.device, &module_info, nullptr, &module) != VK_SUCCESS) {
            throw std::runtime_error("failed to create compute module");
        }

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = 1;
        pipeline_layout_info.pSetLayouts = &descriptor_layout_;
        pipeline_layout_info.pushConstantRangeCount = 1;
        pipeline_layout_info.pPushConstantRanges = &push_range;
        if (vkCreatePipelineLayout(ctx_.device, &pipeline_layout_info, nullptr, &pipeline_layout_) !=
            VK_SUCCESS) {
            vkDestroyShaderModule(ctx_.device, module, nullptr);
            throw std::runtime_error("failed to create pipeline layout");
        }

        VkPipelineShaderStageCreateInfo stage{};
        stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        stage.module = module;
        stage.pName = "main";

        VkComputePipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipeline_info.stage = stage;
        pipeline_info.layout = pipeline_layout_;
        if (vkCreateComputePipelines(ctx_.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr,
                                     &compute_pipeline_) != VK_SUCCESS) {
            vkDestroyShaderModule(ctx_.device, module, nullptr);
            throw std::runtime_error("failed to create compute pipeline");
        }
        vkDestroyShaderModule(ctx_.device, module, nullptr);
        return true;
    } catch (const std::exception& ex) {
        std::cerr << "[upscale] compute init failed: " << ex.what() << '\n';
        shutdown_compute();
        return false;
    }
}

void HostFramePipeline::shutdown_compute() {
    if (compute_pipeline_ != VK_NULL_HANDLE) {
        vkDestroyPipeline(ctx_.device, compute_pipeline_, nullptr);
        compute_pipeline_ = VK_NULL_HANDLE;
    }
    if (pipeline_layout_ != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(ctx_.device, pipeline_layout_, nullptr);
        pipeline_layout_ = VK_NULL_HANDLE;
    }
    if (descriptor_pool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(ctx_.device, descriptor_pool_, nullptr);
        descriptor_pool_ = VK_NULL_HANDLE;
        descriptor_set_ = VK_NULL_HANDLE;
    }
    if (descriptor_layout_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(ctx_.device, descriptor_layout_, nullptr);
        descriptor_layout_ = VK_NULL_HANDLE;
    }
    if (sampler_ != VK_NULL_HANDLE) {
        vkDestroySampler(ctx_.device, sampler_, nullptr);
        sampler_ = VK_NULL_HANDLE;
    }
}

bool HostFramePipeline::init_dlss() {
    if (dlss_ == nullptr) {
        dlss_ = new DlssUpscaler();
    }
    return dlss_->init(instance_, ctx_, ctx_.swapchain_format, render_extent_, output_extent_);
}

void HostFramePipeline::shutdown_dlss() {
    if (dlss_ != nullptr) {
        dlss_->shutdown();
        delete dlss_;
        dlss_ = nullptr;
    }
}

void HostFramePipeline::record_upscale(VkCommandBuffer cmd,
                                       VkImage swapchain_image,
                                       VkImageView swapchain_view,
                                       VkExtent2D output_extent) {
    if (!active_) {
        return;
    }

    if (backend_in_use_ == UpscaleBackend::DLSS && dlss_ != nullptr && dlss_->active()) {
        record_dlss_upscale(cmd, swapchain_image, swapchain_view, output_extent);
        return;
    }
    record_compute_upscale(cmd, swapchain_image, swapchain_view, output_extent);
}

void HostFramePipeline::record_compute_upscale(VkCommandBuffer cmd,
                                               VkImage swapchain_image,
                                               VkImageView swapchain_view,
                                               VkExtent2D output_extent) {
    cmd_image_barrier(cmd,
                      offscreen_image_,
                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                      VK_ACCESS_SHADER_READ_BIT,
                      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                      VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

    cmd_image_barrier(cmd,
                      resolve_image_,
                      VK_IMAGE_LAYOUT_UNDEFINED,
                      VK_IMAGE_LAYOUT_GENERAL,
                      0,
                      VK_ACCESS_SHADER_WRITE_BIT,
                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                      VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

    struct UpscaleParams {
        uint32_t src_size[2];
        uint32_t dst_size[2];
        float sharpen;
    } params{};
    params.src_size[0] = render_extent_.width;
    params.src_size[1] = render_extent_.height;
    params.dst_size[0] = output_extent.width;
    params.dst_size[1] = output_extent.height;
    params.sharpen = 1.25f;

    VkDescriptorImageInfo src_info{};
    src_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    src_info.imageView = offscreen_view_;
    src_info.sampler = sampler_;

    VkDescriptorImageInfo dst_info{};
    dst_info.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    dst_info.imageView = resolve_view_;

    VkWriteDescriptorSet writes[2]{};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstSet = descriptor_set_;
    writes[0].dstBinding = 0;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[0].descriptorCount = 1;
    writes[0].pImageInfo = &src_info;
    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstSet = descriptor_set_;
    writes[1].dstBinding = 1;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    writes[1].descriptorCount = 1;
    writes[1].pImageInfo = &dst_info;
    vkUpdateDescriptorSets(ctx_.device, 2, writes, 0, nullptr);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, compute_pipeline_);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline_layout_, 0, 1, &descriptor_set_,
                            0, nullptr);
    vkCmdPushConstants(cmd, pipeline_layout_, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(params), &params);

    const uint32_t group_x = (output_extent.width + 7) / 8;
    const uint32_t group_y = (output_extent.height + 7) / 8;
    vkCmdDispatch(cmd, group_x, group_y, 1);

    cmd_image_barrier(cmd,
                      resolve_image_,
                      VK_IMAGE_LAYOUT_GENERAL,
                      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                      VK_ACCESS_SHADER_WRITE_BIT,
                      VK_ACCESS_TRANSFER_READ_BIT,
                      VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                      VK_PIPELINE_STAGE_TRANSFER_BIT);

    cmd_image_barrier(cmd,
                      swapchain_image,
                      VK_IMAGE_LAYOUT_UNDEFINED,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      0,
                      VK_ACCESS_TRANSFER_WRITE_BIT,
                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                      VK_PIPELINE_STAGE_TRANSFER_BIT);

    VkImageBlit blit{};
    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.srcSubresource.layerCount = 1;
    blit.srcOffsets[1].x = static_cast<int32_t>(output_extent.width);
    blit.srcOffsets[1].y = static_cast<int32_t>(output_extent.height);
    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.dstSubresource.layerCount = 1;
    blit.dstOffsets[1].x = static_cast<int32_t>(output_extent.width);
    blit.dstOffsets[1].y = static_cast<int32_t>(output_extent.height);
    vkCmdBlitImage(cmd,
                   resolve_image_,
                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   swapchain_image,
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1,
                   &blit,
                   VK_FILTER_LINEAR);

    cmd_image_barrier(cmd,
                      swapchain_image,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                      VK_ACCESS_TRANSFER_WRITE_BIT,
                      0,
                      VK_PIPELINE_STAGE_TRANSFER_BIT,
                      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
}

void HostFramePipeline::record_dlss_upscale(VkCommandBuffer cmd,
                                            VkImage swapchain_image,
                                            VkImageView swapchain_view,
                                            VkExtent2D output_extent) {
    (void)swapchain_view;
    if (dlss_ == nullptr) {
        return;
    }
    dlss_->evaluate(cmd,
                    offscreen_view_,
                    offscreen_image_,
                    resolve_view_,
                    resolve_image_,
                    render_extent_,
                    output_extent);

    cmd_image_barrier(cmd,
                      resolve_image_,
                      VK_IMAGE_LAYOUT_GENERAL,
                      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                      VK_ACCESS_SHADER_WRITE_BIT,
                      VK_ACCESS_TRANSFER_READ_BIT,
                      VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                      VK_PIPELINE_STAGE_TRANSFER_BIT);

    cmd_image_barrier(cmd,
                      swapchain_image,
                      VK_IMAGE_LAYOUT_UNDEFINED,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      0,
                      VK_ACCESS_TRANSFER_WRITE_BIT,
                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                      VK_PIPELINE_STAGE_TRANSFER_BIT);

    VkImageBlit blit{};
    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.srcSubresource.layerCount = 1;
    blit.srcOffsets[1].x = static_cast<int32_t>(output_extent.width);
    blit.srcOffsets[1].y = static_cast<int32_t>(output_extent.height);
    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.dstSubresource.layerCount = 1;
    blit.dstOffsets[1].x = static_cast<int32_t>(output_extent.width);
    blit.dstOffsets[1].y = static_cast<int32_t>(output_extent.height);
    vkCmdBlitImage(cmd,
                   resolve_image_,
                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   swapchain_image,
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1,
                   &blit,
                   VK_FILTER_LINEAR);

    cmd_image_barrier(cmd,
                      swapchain_image,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                      VK_ACCESS_TRANSFER_WRITE_BIT,
                      0,
                      VK_PIPELINE_STAGE_TRANSFER_BIT,
                      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
}

} // namespace host
