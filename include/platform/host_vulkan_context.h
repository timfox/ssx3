#ifndef PLATFORM_HOST_VULKAN_CONTEXT_H
#define PLATFORM_HOST_VULKAN_CONTEXT_H

#include <vulkan/vulkan.h>

namespace host {

struct VulkanDeviceContext {
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphics_queue = VK_NULL_HANDLE;
    uint32_t graphics_family = 0;
    VkCommandPool command_pool = VK_NULL_HANDLE;
    VkFormat swapchain_format = VK_FORMAT_UNDEFINED;
};

void cmd_image_barrier(VkCommandBuffer cmd,
                       VkImage image,
                       VkImageLayout old_layout,
                       VkImageLayout new_layout,
                       VkAccessFlags src_access,
                       VkAccessFlags dst_access,
                       VkPipelineStageFlags src_stage,
                       VkPipelineStageFlags dst_stage);

uint32_t find_memory_type(VkPhysicalDevice physical_device,
                          uint32_t type_filter,
                          VkMemoryPropertyFlags properties);

void create_image(VkPhysicalDevice physical_device,
                  VkDevice device,
                  uint32_t width,
                  uint32_t height,
                  VkFormat format,
                  VkImageUsageFlags usage,
                  VkMemoryPropertyFlags properties,
                  VkImage& image,
                  VkDeviceMemory& memory);

void create_image_view(VkDevice device,
                       VkImage image,
                       VkFormat format,
                       VkImageAspectFlags aspect,
                       VkImageView& view);

} // namespace host

#endif /* PLATFORM_HOST_VULKAN_CONTEXT_H */
