#include "platform/host_dlss.h"
#include "platform/host_vulkan_context.h"

#ifdef SSX3_HAVE_DLSS

#include <nvsdk_ngx.h>
#include <nvsdk_ngx_defs.h>
#include <nvsdk_ngx_helpers_vk.h>
#include <nvsdk_ngx_params.h>
#include <nvsdk_ngx_vk.h>

#include <cstring>
#include <iostream>
#include <vector>

namespace host {
namespace {

VkImageSubresourceRange color_subresource_range() {
    VkImageSubresourceRange range{};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;
    return range;
}

VkImageSubresourceRange depth_subresource_range() {
    VkImageSubresourceRange range{};
    range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;
    return range;
}

} // namespace

struct DlssUpscaler::Impl {
    VulkanDeviceContext ctx{};
    VkInstance instance = VK_NULL_HANDLE;
    VkFormat color_format = VK_FORMAT_UNDEFINED;
    VkExtent2D render_extent{};
    VkExtent2D output_extent{};

    NVSDK_NGX_Handle* dlss_handle = nullptr;
    NVSDK_NGX_Parameter* ngx_params = nullptr;

    VkImage depth_image = VK_NULL_HANDLE;
    VkDeviceMemory depth_memory = VK_NULL_HANDLE;
    VkImageView depth_view = VK_NULL_HANDLE;

    VkImage motion_image = VK_NULL_HANDLE;
    VkDeviceMemory motion_memory = VK_NULL_HANDLE;
    VkImageView motion_view = VK_NULL_HANDLE;

    VkCommandBuffer setup_cmd = VK_NULL_HANDLE;
    bool ngx_initialized = false;
};

bool DlssUpscaler::init(VkInstance instance,
                        const VulkanDeviceContext& ctx,
                        VkFormat color_format,
                        VkExtent2D render_extent,
                        VkExtent2D output_extent) {
    shutdown();
    impl_ = new Impl();
    impl_->instance = instance;
    impl_->ctx = ctx;
    impl_->color_format = color_format;
    impl_->render_extent = render_extent;
    impl_->output_extent = output_extent;

    const wchar_t* log_path = L".";
    const NVSDK_NGX_Result init_result = NVSDK_NGX_VULKAN_Init_with_ProjectID(
        "ssx3-native",
        NVSDK_NGX_ENGINE_TYPE_CUSTOM,
        "0.1.0",
        log_path,
        instance,
        ctx.physical_device,
        ctx.device,
        nullptr,
        nullptr,
        nullptr,
        NVSDK_NGX_Version_API);
    if (init_result != NVSDK_NGX_Result_Success) {
        std::cerr << "[dlss]    NGX init failed: " << init_result << '\n';
        shutdown();
        return false;
    }
    impl_->ngx_initialized = true;

    if (NVSDK_NGX_VULKAN_GetCapabilityParameters(&impl_->ngx_params) != NVSDK_NGX_Result_Success) {
        std::cerr << "[dlss]    NGX GetCapabilityParameters failed\n";
        shutdown();
        return false;
    }

    try {
        create_image(ctx.physical_device,
                     ctx.device,
                     render_extent.width,
                     render_extent.height,
                     VK_FORMAT_D32_SFLOAT,
                     VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                     impl_->depth_image,
                     impl_->depth_memory);
        create_image_view(ctx.device, impl_->depth_image, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT,
                          impl_->depth_view);

        create_image(ctx.physical_device,
                     ctx.device,
                     render_extent.width,
                     render_extent.height,
                     VK_FORMAT_R16G16_SFLOAT,
                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                     impl_->motion_image,
                     impl_->motion_memory);
        create_image_view(ctx.device, impl_->motion_image, VK_FORMAT_R16G16_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT,
                          impl_->motion_view);
    } catch (const std::exception& ex) {
        std::cerr << "[dlss]    auxiliary buffer setup failed: " << ex.what() << '\n';
        shutdown();
        return false;
    }

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = ctx.command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;
    if (vkAllocateCommandBuffers(ctx.device, &alloc_info, &impl_->setup_cmd) != VK_SUCCESS) {
        shutdown();
        return false;
    }

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(impl_->setup_cmd, &begin_info);

    NVSDK_NGX_DLSS_Create_Params create_params{};
    create_params.Feature.InWidth = render_extent.width;
    create_params.Feature.InHeight = render_extent.height;
    create_params.Feature.InTargetWidth = output_extent.width;
    create_params.Feature.InTargetHeight = output_extent.height;
    create_params.Feature.InPerfQualityValue = NVSDK_NGX_PerfQuality_Value_Balanced;
    create_params.InFeatureCreateFlags = 0;
    create_params.InEnableOutputSubrects = false;

    const NVSDK_NGX_Result create_result = NGX_VULKAN_CREATE_DLSS_EXT1(
        ctx.device,
        impl_->setup_cmd,
        1,
        1,
        &impl_->dlss_handle,
        impl_->ngx_params,
        &create_params);
    if (create_result != NVSDK_NGX_Result_Success) {
        std::cerr << "[dlss]    NGX_VULKAN_CREATE_DLSS failed: " << create_result << '\n';
        shutdown();
        return false;
    }

    vkEndCommandBuffer(impl_->setup_cmd);
    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &impl_->setup_cmd;
    vkQueueSubmit(ctx.graphics_queue, 1, &submit, VK_NULL_HANDLE);
    vkQueueWaitIdle(ctx.graphics_queue);

    active_ = true;
    std::cout << "[dlss]    NGX Super Resolution enabled\n";
    return true;
}

void DlssUpscaler::shutdown() {
    if (impl_ == nullptr) {
        active_ = false;
        return;
    }

  if (impl_->ctx.device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(impl_->ctx.device);
    }

    if (impl_->dlss_handle != nullptr && impl_->ngx_initialized) {
        NVSDK_NGX_VULKAN_ReleaseFeature(impl_->dlss_handle);
        impl_->dlss_handle = nullptr;
    }

    if (impl_->setup_cmd != VK_NULL_HANDLE && impl_->ctx.device != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(impl_->ctx.device, impl_->ctx.command_pool, 1, &impl_->setup_cmd);
        impl_->setup_cmd = VK_NULL_HANDLE;
    }

    if (impl_->depth_view != VK_NULL_HANDLE) {
        vkDestroyImageView(impl_->ctx.device, impl_->depth_view, nullptr);
        impl_->depth_view = VK_NULL_HANDLE;
    }
    if (impl_->depth_image != VK_NULL_HANDLE) {
        vkDestroyImage(impl_->ctx.device, impl_->depth_image, nullptr);
        impl_->depth_image = VK_NULL_HANDLE;
    }
    if (impl_->depth_memory != VK_NULL_HANDLE) {
        vkFreeMemory(impl_->ctx.device, impl_->depth_memory, nullptr);
        impl_->depth_memory = VK_NULL_HANDLE;
    }

    if (impl_->motion_view != VK_NULL_HANDLE) {
        vkDestroyImageView(impl_->ctx.device, impl_->motion_view, nullptr);
        impl_->motion_view = VK_NULL_HANDLE;
    }
    if (impl_->motion_image != VK_NULL_HANDLE) {
        vkDestroyImage(impl_->ctx.device, impl_->motion_image, nullptr);
        impl_->motion_image = VK_NULL_HANDLE;
    }
    if (impl_->motion_memory != VK_NULL_HANDLE) {
        vkFreeMemory(impl_->ctx.device, impl_->motion_memory, nullptr);
        impl_->motion_memory = VK_NULL_HANDLE;
    }

    if (impl_->ngx_initialized) {
        NVSDK_NGX_VULKAN_Shutdown1(impl_->ctx.device);
        impl_->ngx_initialized = false;
    }

    delete impl_;
    impl_ = nullptr;
    active_ = false;
}

void DlssUpscaler::resize(VkExtent2D render_extent, VkExtent2D output_extent) {
    if (!active_ || impl_ == nullptr) {
        return;
    }
    const auto instance = impl_->instance;
    const auto ctx = impl_->ctx;
    const auto format = impl_->color_format;
    shutdown();
    init(instance, ctx, format, render_extent, output_extent);
}

void DlssUpscaler::evaluate(VkCommandBuffer cmd,
                            VkImageView input_color,
                            VkImage input_color_image,
                            VkImageView output_color,
                            VkImage output_color_image,
                            VkExtent2D render_extent,
                            VkExtent2D output_extent) {
    if (!active_ || impl_ == nullptr || impl_->dlss_handle == nullptr) {
        return;
    }

    cmd_image_barrier(cmd,
                      input_color_image,
                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                      VK_ACCESS_SHADER_READ_BIT,
                      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                      VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

    cmd_image_barrier(cmd,
                      output_color_image,
                      VK_IMAGE_LAYOUT_UNDEFINED,
                      VK_IMAGE_LAYOUT_GENERAL,
                      0,
                      VK_ACCESS_SHADER_WRITE_BIT,
                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                      VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

    const auto color_range = color_subresource_range();
    NVSDK_NGX_Resource_VK in_color = NVSDK_NGX_Create_ImageView_Resource_VK(
        input_color,
        input_color_image,
        color_range,
        impl_->color_format,
        render_extent.width,
        render_extent.height,
        false);

    NVSDK_NGX_Resource_VK out_color = NVSDK_NGX_Create_ImageView_Resource_VK(
        output_color,
        output_color_image,
        color_range,
        impl_->color_format,
        output_extent.width,
        output_extent.height,
        true);

    const auto depth_range = depth_subresource_range();
    NVSDK_NGX_Resource_VK depth = NVSDK_NGX_Create_ImageView_Resource_VK(
        impl_->depth_view,
        impl_->depth_image,
        depth_range,
        VK_FORMAT_D32_SFLOAT,
        render_extent.width,
        render_extent.height,
        false);

    const auto mv_range = color_subresource_range();
    NVSDK_NGX_Resource_VK motion = NVSDK_NGX_Create_ImageView_Resource_VK(
        impl_->motion_view,
        impl_->motion_image,
        mv_range,
        VK_FORMAT_R16G16_SFLOAT,
        render_extent.width,
        render_extent.height,
        false);

    NVSDK_NGX_VK_DLSS_Eval_Params eval_params{};
    eval_params.Feature.pInColor = &in_color;
    eval_params.Feature.pInOutput = &out_color;
    eval_params.pInDepth = &depth;
    eval_params.pInMotionVectors = &motion;
    eval_params.InRenderSubrectDimensions.Width = render_extent.width;
    eval_params.InRenderSubrectDimensions.Height = render_extent.height;
    eval_params.InJitterOffsetX = 0.0f;
    eval_params.InJitterOffsetY = 0.0f;
    eval_params.InMVScaleX = static_cast<float>(render_extent.width);
    eval_params.InMVScaleY = static_cast<float>(render_extent.height);
    eval_params.InReset = 0;
    eval_params.Feature.InSharpness = 0.5f;

    const NVSDK_NGX_Result eval_result =
        NGX_VULKAN_EVALUATE_DLSS_EXT(cmd, impl_->dlss_handle, impl_->ngx_params, &eval_params);
    if (eval_result != NVSDK_NGX_Result_Success) {
        std::cerr << "[dlss]    evaluate failed: " << eval_result << '\n';
    }

    (void)output_color_image;
}

} // namespace host

#else
#error "host_dlss_upscale.cpp requires SSX3_HAVE_DLSS"
#endif
