#ifndef PLATFORM_HOST_DLSS_H
#define PLATFORM_HOST_DLSS_H

#include "platform/host_vulkan_context.h"

#include <vulkan/vulkan.h>

struct VkInstance_T;
using VkInstance = VkInstance_T*;

namespace host {

struct DlssUpscaler {
    bool init(VkInstance instance,
              const VulkanDeviceContext& ctx,
              VkFormat color_format,
              VkExtent2D render_extent,
              VkExtent2D output_extent);
    void shutdown();
    bool active() const { return active_; }

    void resize(VkExtent2D render_extent, VkExtent2D output_extent);
    void evaluate(VkCommandBuffer cmd,
                  VkImageView input_color,
                  VkImage input_color_image,
                  VkImageView output_color,
                  VkImage output_color_image,
                  VkExtent2D render_extent,
                  VkExtent2D output_extent);

private:
    bool active_ = false;
    struct Impl;
    Impl* impl_ = nullptr;
};

} // namespace host

#endif /* PLATFORM_HOST_DLSS_H */
