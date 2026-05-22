#ifndef PLATFORM_HOST_VULKAN_WORLD_H
#define PLATFORM_HOST_VULKAN_WORLD_H

#include "platform/host_world_scene.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace host {

class VulkanWorldGpu {
public:
    bool init(VkPhysicalDevice physical_device,
              VkDevice device,
              VkQueue graphics_queue,
              uint32_t graphics_family,
              VkCommandPool command_pool,
              VkFormat swap_format,
              const std::vector<VkImageView>& swap_views,
              VkExtent2D extent,
              const std::string& shader_dir,
              const WorldScene& scene);
    void shutdown();

    void on_swapchain_changed(const std::vector<VkImageView>& swap_views, VkExtent2D extent);

    void set_camera(const WorldCamera& camera, float aspect);
    void record(VkCommandBuffer cmd, std::uint32_t swap_image_index, VkExtent2D extent);

private:
    bool create_render_pass();
    bool create_depth_resources();
    bool create_framebuffers(const std::vector<VkImageView>& swap_views);
    bool create_pipeline();
    bool upload_mesh(const WorldMesh& mesh,
                     VkBuffer& vertex_buffer,
                     VkDeviceMemory& vertex_memory,
                     VkBuffer& index_buffer,
                     VkDeviceMemory& index_memory,
                     std::uint32_t& index_count);
    void destroy_framebuffers();
    void destroy_depth();
    void destroy_pipeline();
    void destroy_mesh();
    void destroy_mesh_buffers(VkBuffer& vertex_buffer,
                              VkDeviceMemory& vertex_memory,
                              VkBuffer& index_buffer,
                              VkDeviceMemory& index_memory,
                              std::uint32_t& index_count);

    VkShaderModule create_shader_module(const std::vector<char>& code) const;
    uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
    void create_buffer(VkDeviceSize size,
                       VkBufferUsageFlags usage,
                       VkMemoryPropertyFlags properties,
                       VkBuffer& buffer,
                       VkDeviceMemory& memory) const;

    VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
    VkQueue graphics_queue_ = VK_NULL_HANDLE;
    uint32_t graphics_family_ = 0;
    VkCommandPool command_pool_ = VK_NULL_HANDLE;
    VkFormat swap_format_ = VK_FORMAT_UNDEFINED;
    VkFormat depth_format_ = VK_FORMAT_UNDEFINED;
    std::string shader_dir_;
    VkExtent2D extent_{};

    VkRenderPass render_pass_ = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> framebuffers_;
    VkImage depth_image_ = VK_NULL_HANDLE;
    VkDeviceMemory depth_memory_ = VK_NULL_HANDLE;
    VkImageView depth_view_ = VK_NULL_HANDLE;

    VkPipeline pipeline_ = VK_NULL_HANDLE;
    VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptor_layout_ = VK_NULL_HANDLE;
    VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
    VkDescriptorSet descriptor_set_ = VK_NULL_HANDLE;

    VkBuffer uniform_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory uniform_memory_ = VK_NULL_HANDLE;
    void* uniform_mapped_ = nullptr;

    VkBuffer vertex_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory vertex_memory_ = VK_NULL_HANDLE;
    VkBuffer index_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory index_memory_ = VK_NULL_HANDLE;
    std::uint32_t index_count_ = 0;

    VkBuffer marker_vertex_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory marker_vertex_memory_ = VK_NULL_HANDLE;
    VkBuffer marker_index_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory marker_index_memory_ = VK_NULL_HANDLE;
    std::uint32_t marker_index_count_ = 0;
};

} // namespace host

#endif /* PLATFORM_HOST_VULKAN_WORLD_H */
