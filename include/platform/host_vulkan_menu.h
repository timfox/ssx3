#ifndef PLATFORM_HOST_VULKAN_MENU_H
#define PLATFORM_HOST_VULKAN_MENU_H

#include "platform/host_menu_frame.h"

#include <vulkan/vulkan.h>

#include <cstdint>
#include <string>
#include <vector>

namespace host {

/** True after a successful texture upload for this GPU slot. */
bool vulkan_menu_texture_ready(int texture_index);

class VulkanMenuGpu {
public:
    bool init(VkPhysicalDevice physical_device,
              VkDevice device,
              VkQueue graphics_queue,
              uint32_t graphics_family,
              VkRenderPass render_pass,
              VkFormat swap_format,
              const std::string& shader_dir);
    void shutdown();

    bool load_assets(const std::string& asset_dir);
    void on_swapchain_resize(VkExtent2D extent);
    void on_render_pass_changed(VkRenderPass render_pass);

    void record(VkCommandBuffer cmd, VkExtent2D draw_extent, const MenuFrame& frame);

private:
    struct GpuTexture {
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        int width = 0;
        int height = 0;
    };

    bool create_pipeline();
    void destroy_pipeline();
    bool load_png_texture(const std::string& path, int index);
    bool load_rgba_texture(const unsigned char* pixels, int width, int height, int index);
    bool create_snow_texture();
    bool create_solid_texture();
    void screen_to_ndc(float sx, float sy, float sw, float sh, VkExtent2D extent, float out[8]) const;
    void append_sprite_quad(const MenuSprite& sprite,
                            VkExtent2D extent,
                            std::vector<float>& verts) const;
    uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
    void create_buffer(VkDeviceSize size,
                       VkBufferUsageFlags usage,
                       VkMemoryPropertyFlags properties,
                       VkBuffer& buffer,
                       VkDeviceMemory& memory) const;
    VkShaderModule create_shader_module(const std::vector<char>& code) const;

    VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
    VkQueue graphics_queue_ = VK_NULL_HANDLE;
    uint32_t graphics_family_ = 0;
    VkRenderPass render_pass_ = VK_NULL_HANDLE;
    VkFormat swap_format_ = VK_FORMAT_UNDEFINED;
    std::string shader_dir_;

    VkPipeline pipeline_ = VK_NULL_HANDLE;
    VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptor_layout_ = VK_NULL_HANDLE;
    VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptor_sets_;

    VkBuffer vertex_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory vertex_buffer_memory_ = VK_NULL_HANDLE;
    VkDeviceSize vertex_buffer_capacity_ = 0;

    std::vector<GpuTexture> textures_;
    VkExtent2D extent_{};
};

} // namespace host

#endif /* PLATFORM_HOST_VULKAN_MENU_H */
