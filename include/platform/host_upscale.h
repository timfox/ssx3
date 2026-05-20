#ifndef PLATFORM_HOST_UPSCALE_H
#define PLATFORM_HOST_UPSCALE_H

#include "platform/host_vulkan_context.h"

#include <vulkan/vulkan.h>

struct VkInstance_T;
using VkInstance = VkInstance_T*;

namespace host {

enum class UpscaleBackend {
    Direct,
    Compute,
    DLSS,
};

struct UpscaleSettings {
    UpscaleBackend backend = UpscaleBackend::Compute;
    float render_scale = 0.67f;
    const char* shader_dir = "obj/host/shaders";
};

class HostFramePipeline {
public:
    bool init(VkInstance instance,
              const VulkanDeviceContext& ctx,
              const char* shader_dir,
              VkRenderPass shared_render_pass,
              UpscaleSettings settings,
              VkExtent2D output_extent);
    void shutdown();

    void resize(VkExtent2D output_extent);

    bool active() const { return active_; }
    VkExtent2D render_extent() const { return render_extent_; }
    VkFramebuffer render_framebuffer() const { return offscreen_framebuffer_; }

    void record_upscale(VkCommandBuffer cmd,
                        VkImage swapchain_image,
                        VkImageView swapchain_view,
                        VkExtent2D output_extent);

    const char* active_backend_name() const;

private:
    bool create_offscreen_target();
    void destroy_offscreen_target();
    bool init_compute(const char* shader_dir);
    void shutdown_compute();
    bool init_dlss();
    void shutdown_dlss();
    void record_compute_upscale(VkCommandBuffer cmd,
                                VkImage swapchain_image,
                                VkImageView swapchain_view,
                                VkExtent2D output_extent);
    void record_dlss_upscale(VkCommandBuffer cmd,
                             VkImage swapchain_image,
                             VkImageView swapchain_view,
                             VkExtent2D output_extent);

    VkInstance instance_ = VK_NULL_HANDLE;
    VulkanDeviceContext ctx_{};
    UpscaleSettings settings_{};
    VkRenderPass render_pass_ = VK_NULL_HANDLE;
    bool active_ = false;
    UpscaleBackend backend_in_use_ = UpscaleBackend::Direct;

    VkExtent2D output_extent_{};
    VkExtent2D render_extent_{};

    VkImage offscreen_image_ = VK_NULL_HANDLE;
    VkDeviceMemory offscreen_memory_ = VK_NULL_HANDLE;
    VkImageView offscreen_view_ = VK_NULL_HANDLE;
    VkFramebuffer offscreen_framebuffer_ = VK_NULL_HANDLE;

    VkImage resolve_image_ = VK_NULL_HANDLE;
    VkDeviceMemory resolve_memory_ = VK_NULL_HANDLE;
    VkImageView resolve_view_ = VK_NULL_HANDLE;

    VkSampler sampler_ = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptor_layout_ = VK_NULL_HANDLE;
    VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
    VkDescriptorSet descriptor_set_ = VK_NULL_HANDLE;
    VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
    VkPipeline compute_pipeline_ = VK_NULL_HANDLE;

    class DlssUpscaler* dlss_ = nullptr;
};

} // namespace host

#endif /* PLATFORM_HOST_UPSCALE_H */
