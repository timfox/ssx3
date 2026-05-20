#include "platform/host_renderer.h"
#include "platform/host_gs.h"
#include "platform/host_upscale.h"
#include "platform/host_video_player.h"
#include "platform/host_vulkan_context.h"
#include "platform/host_vulkan_menu.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace host {
namespace {

constexpr int kMaxFramesInFlight = 2;

struct Vertex {
    float pos[2];
    float color[3];
};

const std::array<Vertex, 3> kTriangleVertices = {{
    {{0.0f, -0.5f}, {1.0f, 0.2f, 0.2f}},
    {{0.5f, 0.5f}, {0.2f, 1.0f, 0.3f}},
    {{-0.5f, 0.5f}, {0.2f, 0.4f, 1.0f}},
}};

[[noreturn]] void throw_vk(const char* what) {
    throw std::runtime_error(what);
}

std::vector<char> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file) {
        throw_vk(("failed to open file: " + path).c_str());
    }
    const auto size = file.tellg();
    std::vector<char> buffer(static_cast<size_t>(size));
    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;

    bool complete() const { return graphics.has_value() && present.has_value(); }
};

struct SwapchainSupport {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

class VulkanRenderer final : public Renderer {
public:
    bool init(const RendererConfig& config) override {
        config_ = config;
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
            return false;
        }

        window_ = SDL_CreateWindow(
            config_.title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            config_.width,
            config_.height,
            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
        if (!window_) {
            std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
            return false;
        }
        SDL_SetWindowMinimumSize(window_, 320, 224);

        try {
            create_instance();
            create_surface();
            pick_physical_device();
            create_logical_device();
            create_swapchain();
            create_image_views();
            create_render_pass();
            create_graphics_pipeline();
            create_framebuffers();
            create_command_pool();
            create_vertex_buffer();
            create_command_buffers();
            create_sync_objects();
            create_video_resources();

            if (config_.upscale != UpscaleBackend::Direct) {
                VulkanDeviceContext device_ctx{};
                device_ctx.physical_device = physical_device_;
                device_ctx.device = device_;
                device_ctx.graphics_queue = graphics_queue_;
                device_ctx.graphics_family = *queue_indices_.graphics;
                device_ctx.command_pool = command_pool_;
                device_ctx.swapchain_format = swap_format_;

                UpscaleSettings upscale{};
                upscale.backend = config_.upscale;
                upscale.render_scale = config_.render_scale;
                upscale.shader_dir = config_.shader_dir;

                if (!frame_pipeline_.init(instance_,
                                          device_ctx,
                                          config_.shader_dir,
                                          render_pass_,
                                          upscale,
                                          swap_extent_)) {
                    std::cerr << "[gfx]     upscale init failed; using direct rendering\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cerr << "Vulkan init failed: " << ex.what() << '\n';
            shutdown();
            return false;
        }

        if (frame_pipeline_.active()) {
            std::cout << "[gfx]     Vulkan renderer ready (" << swap_extent_.width << "x"
                      << swap_extent_.height << ", render "
                      << frame_pipeline_.render_extent().width << "x"
                      << frame_pipeline_.render_extent().height << ", upscale "
                      << frame_pipeline_.active_backend_name() << ")\n";
        } else {
            std::cout << "[gfx]     Vulkan renderer ready (" << swap_extent_.width << "x"
                      << swap_extent_.height << ")\n";
        }
        return true;
    }

    void shutdown() override {
        if (device_ != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(device_);
        }

        shutdown_main_menu();
        frame_pipeline_.shutdown();
        destroy_video_resources();
        cleanup_swapchain();

        if (vertex_buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(device_, vertex_buffer_, nullptr);
        }
        if (vertex_buffer_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(device_, vertex_buffer_memory_, nullptr);
        }
        if (command_pool_ != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device_, command_pool_, nullptr);
            command_pool_ = VK_NULL_HANDLE;
        }
        if (device_ != VK_NULL_HANDLE) {
            vkDestroyDevice(device_, nullptr);
            device_ = VK_NULL_HANDLE;
        }
        if (surface_ != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(instance_, surface_, nullptr);
            surface_ = VK_NULL_HANDLE;
        }
        if (instance_ != VK_NULL_HANDLE) {
            vkDestroyInstance(instance_, nullptr);
            instance_ = VK_NULL_HANDLE;
        }
        if (window_) {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }
        SDL_Quit();
    }

    bool begin_frame() override {
        poll_events();
        if (should_close()) {
            return false;
        }

        if (framebuffer_resized_) {
            recreate_swapchain();
            framebuffer_resized_ = false;
        }

        vkWaitForFences(device_, 1, &in_flight_fence_, VK_TRUE, UINT64_MAX);

        uint32_t image_index = 0;
        VkResult acquire = vkAcquireNextImageKHR(
            device_, swapchain_, UINT64_MAX, image_available_semaphore_, VK_NULL_HANDLE, &image_index);
        if (acquire == VK_ERROR_OUT_OF_DATE_KHR) {
            recreate_swapchain();
            return begin_frame();
        }
        if (acquire != VK_SUCCESS && acquire != VK_SUBOPTIMAL_KHR) {
            throw_vk("failed to acquire swapchain image");
        }

        vkResetFences(device_, 1, &in_flight_fence_);
        current_image_ = image_index;

        VkCommandBuffer cmd = command_buffers_[current_image_];
        vkResetCommandBuffer(cmd, 0);

        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(cmd, &begin_info) != VK_SUCCESS) {
            throw_vk("failed to begin command buffer");
        }

        if (swap_image_was_presented_) {
            transition_image(cmd,
                             swap_images_[current_image_],
                             VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                             0,
                             VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
        }

        const bool menu_frame = menu_mode_ && !menu_frame_.sprites.empty();
        const VkExtent2D draw_extent =
            (frame_pipeline_.active() && !menu_frame) ? frame_pipeline_.render_extent() : swap_extent_;
        const VkFramebuffer draw_framebuffer =
            (frame_pipeline_.active() && !menu_frame) ? frame_pipeline_.render_framebuffer()
                                                        : framebuffers_[current_image_];

        VkClearValue clear_color =
            menu_frame ? VkClearValue{{{0.0f, 0.0f, 0.0f, 1.0f}}} : VkClearValue{{{0.05f, 0.07f, 0.12f, 1.0f}}};
        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = render_pass_;
        render_pass_info.framebuffer = draw_framebuffer;
        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = draw_extent;
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(cmd, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        VkViewport viewport{};
        viewport.width = static_cast<float>(draw_extent.width);
        viewport.height = static_cast<float>(draw_extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{};
        scissor.extent = draw_extent;
        vkCmdSetViewport(cmd, 0, 1, &viewport);
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        if (menu_frame) {
            menu_gpu_.record(cmd, draw_extent, menu_frame_);
        } else {
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
            VkBuffer vertex_buffers[] = {vertex_buffer_};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(cmd, 0, 1, vertex_buffers, offsets);
            vkCmdDraw(cmd, 3, 1, 0, 0);
        }
        vkCmdEndRenderPass(cmd);

        if (frame_pipeline_.active() && !menu_frame) {
            frame_pipeline_.record_upscale(cmd,
                                           swap_images_[current_image_],
                                           swap_image_views_[current_image_],
                                           swap_extent_);
        }

        if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
            throw_vk("failed to record command buffer");
        }

        return true;
    }

    void end_frame() override {}

    void present() override {
        VkSemaphore wait_semaphores[] = {image_available_semaphore_};
        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSemaphore signal_semaphores[] = {render_finished_semaphore_};

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffers_[current_image_];
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        if (vkQueueSubmit(graphics_queue_, 1, &submit_info, in_flight_fence_) != VK_SUCCESS) {
            throw_vk("failed to submit draw command buffer");
        }

        VkSwapchainKHR swapchains[] = {swapchain_};
        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;
        present_info.pImageIndices = &current_image_;

        VkResult present = vkQueuePresentKHR(present_queue_, &present_info);
        if (present == VK_ERROR_OUT_OF_DATE_KHR || present == VK_SUBOPTIMAL_KHR || framebuffer_resized_) {
            framebuffer_resized_ = false;
            recreate_swapchain();
        } else if (present != VK_SUCCESS) {
            throw_vk("failed to present swapchain image");
        }

        swap_image_was_presented_ = true;
        current_frame_ = (current_frame_ + 1) % kMaxFramesInFlight;
    }

    bool should_close() const override { return should_close_; }

    void poll_events() override {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                should_close_ = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    should_close_ = true;
                } else if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN ||
                           event.key.keysym.sym == SDLK_KP_ENTER) {
                    skip_video_ = true;
                }
            } else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                    event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    framebuffer_resized_ = true;
                }
            }
        }
    }

    int width() const override { return static_cast<int>(swap_extent_.width); }

    int height() const override { return static_cast<int>(swap_extent_.height); }

    bool present_video_frame(const VideoFrame& frame) override {
        poll_events();
        if (should_close_ || frame.width <= 0 || frame.height <= 0 ||
            frame.rgba.size() < static_cast<size_t>(frame.width * frame.height * 4)) {
            return false;
        }

        if (!ensure_video_resources(frame.width, frame.height)) {
            return false;
        }

        vkWaitForFences(device_, 1, &in_flight_fence_, VK_TRUE, UINT64_MAX);

        uint32_t image_index = 0;
        VkResult acquire = vkAcquireNextImageKHR(
            device_, swapchain_, UINT64_MAX, image_available_semaphore_, VK_NULL_HANDLE, &image_index);
        if (acquire == VK_ERROR_OUT_OF_DATE_KHR) {
            recreate_swapchain();
            destroy_video_resources();
            create_video_resources();
            return present_video_frame(frame);
        }
        if (acquire != VK_SUCCESS && acquire != VK_SUBOPTIMAL_KHR) {
            return false;
        }

        vkResetFences(device_, 1, &in_flight_fence_);
        current_image_ = image_index;

        void* mapped = nullptr;
        const VkDeviceSize upload_size = static_cast<VkDeviceSize>(frame.width * frame.height * 4);
        vkMapMemory(device_, video_staging_memory_, 0, upload_size, 0, &mapped);
        std::memcpy(mapped, frame.rgba.data(), static_cast<size_t>(upload_size));
        vkUnmapMemory(device_, video_staging_memory_);

        VkCommandBuffer cmd = command_buffers_[current_image_];
        vkResetCommandBuffer(cmd, 0);
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(cmd, &begin_info) != VK_SUCCESS) {
            return false;
        }

        transition_image(cmd,
                         video_image_,
                         video_layout_,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                         video_layout_ == VK_IMAGE_LAYOUT_UNDEFINED ? 0 : VK_ACCESS_TRANSFER_READ_BIT,
                         VK_ACCESS_TRANSFER_WRITE_BIT);
        video_layout_ = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

        VkBufferImageCopy region{};
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.layerCount = 1;
        region.imageExtent = {static_cast<uint32_t>(frame.width),
                              static_cast<uint32_t>(frame.height),
                              1};
        vkCmdCopyBufferToImage(cmd,
                               video_staging_buffer_,
                               video_image_,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1,
                               &region);

        transition_image(cmd,
                         video_image_,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                         VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                         VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_TRANSFER_READ_BIT);
        video_layout_ = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

        transition_image(cmd,
                         swap_images_[image_index],
                         VK_IMAGE_LAYOUT_UNDEFINED,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                         0,
                         VK_ACCESS_TRANSFER_WRITE_BIT);

        VkImageBlit blit{};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.layerCount = 1;
        blit.srcOffsets[1] = {frame.width, frame.height, 1};
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.layerCount = 1;
        blit.dstOffsets[1] = {static_cast<int32_t>(swap_extent_.width),
                              static_cast<int32_t>(swap_extent_.height),
                              1};

        vkCmdBlitImage(cmd,
                       video_image_,
                       VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                       swap_images_[image_index],
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       1,
                       &blit,
                       VK_FILTER_LINEAR);

        transition_image(cmd,
                         swap_images_[image_index],
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                         VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                         VK_ACCESS_TRANSFER_WRITE_BIT,
                         0);

        if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
            return false;
        }

        VkSemaphore wait_semaphores[] = {image_available_semaphore_};
        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_TRANSFER_BIT};
        VkSemaphore signal_semaphores[] = {render_finished_semaphore_};
        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmd;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;
        if (vkQueueSubmit(graphics_queue_, 1, &submit_info, in_flight_fence_) != VK_SUCCESS) {
            return false;
        }

        VkSwapchainKHR swapchains[] = {swapchain_};
        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;
        present_info.pImageIndices = &image_index;
        VkResult present = vkQueuePresentKHR(present_queue_, &present_info);
        if (present == VK_ERROR_OUT_OF_DATE_KHR || present == VK_SUBOPTIMAL_KHR || framebuffer_resized_) {
            framebuffer_resized_ = false;
            recreate_swapchain();
            destroy_video_resources();
            create_video_resources();
        } else if (present != VK_SUCCESS) {
            return false;
        }

        swap_image_was_presented_ = true;
        current_frame_ = (current_frame_ + 1) % kMaxFramesInFlight;
        return !should_close_;
    }

    bool consume_skip_video() override {
        if (!skip_video_) {
            return false;
        }
        skip_video_ = false;
        return true;
    }

    bool init_main_menu(const std::string& asset_dir) override {
        if (!menu_gpu_.init(physical_device_,
                            device_,
                            graphics_queue_,
                            *queue_indices_.graphics,
                            render_pass_,
                            swap_format_,
                            config_.shader_dir)) {
            return false;
        }
        if (!menu_gpu_.load_assets(asset_dir)) {
            menu_gpu_.shutdown();
            return false;
        }
        menu_gpu_.on_swapchain_resize(swap_extent_);
        menu_mode_ = true;
        gs_set_renderer(this);
        return true;
    }

    void set_main_menu_frame(const MenuFrame& frame) override { menu_frame_ = frame; }

    void shutdown_main_menu() override {
        gs_set_renderer(nullptr);
        menu_mode_ = false;
        menu_frame_ = {};
        menu_gpu_.shutdown();
    }

private:
    static void transition_image(VkCommandBuffer cmd,
                                 VkImage image,
                                 VkImageLayout old_layout,
                                 VkImageLayout new_layout,
                                 VkAccessFlags src_access,
                                 VkAccessFlags dst_access) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = old_layout;
        barrier.newLayout = new_layout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = src_access;
        barrier.dstAccessMask = dst_access;

        VkPipelineStageFlags src_stage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        VkPipelineStageFlags dst_stage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED) {
            barrier.srcAccessMask = 0;
            src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
        if (new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (new_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
            dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (new_layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
            dst_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        }
        if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
            src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        vkCmdPipelineBarrier(cmd, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    void create_video_resources() {
        video_extent_ = {512, 448, 1};
        ensure_video_resources(static_cast<int>(video_extent_.width), static_cast<int>(video_extent_.height));
    }

    void destroy_video_resources() {
        if (device_ == VK_NULL_HANDLE) {
            return;
        }
        vkDeviceWaitIdle(device_);
        if (video_image_ != VK_NULL_HANDLE) {
            vkDestroyImage(device_, video_image_, nullptr);
            video_image_ = VK_NULL_HANDLE;
        }
        if (video_image_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(device_, video_image_memory_, nullptr);
            video_image_memory_ = VK_NULL_HANDLE;
        }
        if (video_staging_buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(device_, video_staging_buffer_, nullptr);
            video_staging_buffer_ = VK_NULL_HANDLE;
        }
        if (video_staging_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(device_, video_staging_memory_, nullptr);
            video_staging_memory_ = VK_NULL_HANDLE;
        }
        video_layout_ = VK_IMAGE_LAYOUT_UNDEFINED;
        video_extent_ = {0, 0, 1};
    }

    bool ensure_video_resources(int width, int height) {
        if (device_ == VK_NULL_HANDLE || swap_format_ == VK_FORMAT_UNDEFINED) {
            return false;
        }
        if (video_image_ != VK_NULL_HANDLE && static_cast<int>(video_extent_.width) >= width &&
            static_cast<int>(video_extent_.height) >= height) {
            return true;
        }

        destroy_video_resources();
        video_extent_ = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};

        VkImageCreateInfo image_info{};
        image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.imageType = VK_IMAGE_TYPE_2D;
        image_info.extent = video_extent_;
        image_info.mipLevels = 1;
        image_info.arrayLayers = 1;
        image_info.format = swap_format_;
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateImage(device_, &image_info, nullptr, &video_image_) != VK_SUCCESS) {
            return false;
        }

        VkMemoryRequirements mem_requirements{};
        vkGetImageMemoryRequirements(device_, video_image_, &mem_requirements);
        VkMemoryAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = mem_requirements.size;
        alloc_info.memoryTypeIndex =
            find_memory_type(mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        if (vkAllocateMemory(device_, &alloc_info, nullptr, &video_image_memory_) != VK_SUCCESS) {
            return false;
        }
        vkBindImageMemory(device_, video_image_, video_image_memory_, 0);

        const VkDeviceSize staging_size = static_cast<VkDeviceSize>(width * height * 4);
        create_buffer(staging_size,
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      video_staging_buffer_,
                      video_staging_memory_);
        video_layout_ = VK_IMAGE_LAYOUT_UNDEFINED;
        return true;
    }
    void create_instance() {
        uint32_t extension_count = 0;
        if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count, nullptr)) {
            throw_vk("SDL_Vulkan_GetInstanceExtensions count failed");
        }
        std::vector<const char*> extensions(extension_count);
        if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count, extensions.data())) {
            throw_vk("SDL_Vulkan_GetInstanceExtensions failed");
        }

        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "SSX3 Native";
        app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        app_info.pEngineName = "ssx3-host-vulkan";
        app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        app_info.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();

        if (vkCreateInstance(&create_info, nullptr, &instance_) != VK_SUCCESS) {
            throw_vk("failed to create Vulkan instance");
        }
    }

    void create_surface() {
        if (!SDL_Vulkan_CreateSurface(window_, instance_, &surface_)) {
            throw_vk("SDL_Vulkan_CreateSurface failed");
        }
    }

    void pick_physical_device() {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(instance_, &device_count, nullptr);
        if (device_count == 0) {
            throw_vk("no Vulkan physical devices");
        }

        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(instance_, &device_count, devices.data());

        for (const auto device : devices) {
            QueueFamilyIndices indices = find_queue_families(device);
            if (!indices.complete()) {
                continue;
            }
            if (!check_device_extension_support(device)) {
                continue;
            }
            SwapchainSupport support = query_swapchain_support(device);
            if (support.formats.empty() || support.present_modes.empty()) {
                continue;
            }
            physical_device_ = device;
            queue_indices_ = indices;
            return;
        }
        throw_vk("no suitable Vulkan GPU found");
    }

    void create_logical_device() {
        std::set<uint32_t> unique_queues = {*queue_indices_.graphics, *queue_indices_.present};
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        float queue_priority = 1.0f;
        for (uint32_t queue_family : unique_queues) {
            VkDeviceQueueCreateInfo queue_info{};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueFamilyIndex = queue_family;
            queue_info.queueCount = 1;
            queue_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_info);
        }

        VkPhysicalDeviceFeatures device_features{};

        const char* device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.pEnabledFeatures = &device_features;
        create_info.enabledExtensionCount = 1;
        create_info.ppEnabledExtensionNames = device_extensions;

        if (vkCreateDevice(physical_device_, &create_info, nullptr, &device_) != VK_SUCCESS) {
            throw_vk("failed to create logical device");
        }

        vkGetDeviceQueue(device_, *queue_indices_.graphics, 0, &graphics_queue_);
        vkGetDeviceQueue(device_, *queue_indices_.present, 0, &present_queue_);
    }

    void create_swapchain() {
        SwapchainSupport support = query_swapchain_support(physical_device_);
        VkSurfaceFormatKHR surface_format = choose_swap_surface_format(support.formats);
        VkPresentModeKHR present_mode = choose_swap_present_mode(support.present_modes);
        VkExtent2D extent = choose_swap_extent(support.capabilities);

        uint32_t image_count = support.capabilities.minImageCount + 1;
        if (support.capabilities.maxImageCount > 0 &&
            image_count > support.capabilities.maxImageCount) {
            image_count = support.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = surface_;
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        const uint32_t queue_family_indices[] = {*queue_indices_.graphics, *queue_indices_.present};
        if (*queue_indices_.graphics != *queue_indices_.present) {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        } else {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        create_info.preTransform = support.capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = present_mode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = swapchain_;

        if (vkCreateSwapchainKHR(device_, &create_info, nullptr, &swapchain_) != VK_SUCCESS) {
            throw_vk("failed to create swapchain");
        }

        vkGetSwapchainImagesKHR(device_, swapchain_, &image_count, nullptr);
        swap_images_.resize(image_count);
        vkGetSwapchainImagesKHR(device_, swapchain_, &image_count, swap_images_.data());

        swap_format_ = surface_format.format;
        swap_extent_ = extent;
    }

    void create_image_views() {
        swap_image_views_.resize(swap_images_.size());
        for (size_t i = 0; i < swap_images_.size(); ++i) {
            VkImageViewCreateInfo view_info{};
            view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            view_info.image = swap_images_[i];
            view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            view_info.format = swap_format_;
            view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            view_info.subresourceRange.baseMipLevel = 0;
            view_info.subresourceRange.levelCount = 1;
            view_info.subresourceRange.baseArrayLayer = 0;
            view_info.subresourceRange.layerCount = 1;
            if (vkCreateImageView(device_, &view_info, nullptr, &swap_image_views_[i]) != VK_SUCCESS) {
                throw_vk("failed to create image views");
            }
        }
    }

    void create_render_pass() {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = swap_format_;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_ref{};
        color_ref.attachment = 0;
        color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_ref;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.pAttachments = &color_attachment;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        if (vkCreateRenderPass(device_, &render_pass_info, nullptr, &render_pass_) != VK_SUCCESS) {
            throw_vk("failed to create render pass");
        }
    }

    void create_graphics_pipeline() {
        const std::string vert_path = std::string(config_.shader_dir) + "/triangle.vert.spv";
        const std::string frag_path = std::string(config_.shader_dir) + "/triangle.frag.spv";
        auto vert_code = read_file(vert_path);
        auto frag_code = read_file(frag_path);

        VkShaderModule vert_module = create_shader_module(vert_code);
        VkShaderModule frag_module = create_shader_module(frag_code);

        VkPipelineShaderStageCreateInfo vert_stage{};
        vert_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_stage.module = vert_module;
        vert_stage.pName = "main";

        VkPipelineShaderStageCreateInfo frag_stage{};
        frag_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_stage.module = frag_module;
        frag_stage.pName = "main";

        VkPipelineShaderStageCreateInfo stages[] = {vert_stage, frag_stage};

        VkVertexInputBindingDescription binding{};
        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkVertexInputAttributeDescription attributes[2]{};
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[0].offset = offsetof(Vertex, pos);
        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributes[1].offset = offsetof(Vertex, color);

        VkPipelineVertexInputStateCreateInfo vertex_input{};
        vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input.vertexBindingDescriptionCount = 1;
        vertex_input.pVertexBindingDescriptions = &binding;
        vertex_input.vertexAttributeDescriptionCount = 2;
        vertex_input.pVertexAttributeDescriptions = attributes;

        VkPipelineInputAssemblyStateCreateInfo input_assembly{};
        input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swap_extent_.width);
        viewport.height = static_cast<float>(swap_extent_.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swap_extent_;

        VkPipelineViewportStateCreateInfo viewport_state{};
        viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state.viewportCount = 1;
        viewport_state.pViewports = &viewport;
        viewport_state.scissorCount = 1;
        viewport_state.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_NONE;

        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo color_blending{};
        color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blending.logicOpEnable = VK_FALSE;
        color_blending.attachmentCount = 1;
        color_blending.pAttachments = &color_blend_attachment;

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        if (vkCreatePipelineLayout(device_, &pipeline_layout_info, nullptr, &pipeline_layout_) !=
            VK_SUCCESS) {
            throw_vk("failed to create pipeline layout");
        }

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
        pipeline_info.layout = pipeline_layout_;
        pipeline_info.renderPass = render_pass_;
        pipeline_info.subpass = 0;

        VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamic_state{};
        dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state.dynamicStateCount = 2;
        dynamic_state.pDynamicStates = dynamic_states;
        pipeline_info.pDynamicState = &dynamic_state;

        if (vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &pipeline_info, nullptr,
                                      &pipeline_) != VK_SUCCESS) {
            throw_vk("failed to create graphics pipeline");
        }

        vkDestroyShaderModule(device_, frag_module, nullptr);
        vkDestroyShaderModule(device_, vert_module, nullptr);
    }

    void create_framebuffers() {
        framebuffers_.resize(swap_image_views_.size());
        for (size_t i = 0; i < swap_image_views_.size(); ++i) {
            VkImageView attachments[] = {swap_image_views_[i]};
            VkFramebufferCreateInfo framebuffer_info{};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = render_pass_;
            framebuffer_info.attachmentCount = 1;
            framebuffer_info.pAttachments = attachments;
            framebuffer_info.width = swap_extent_.width;
            framebuffer_info.height = swap_extent_.height;
            framebuffer_info.layers = 1;
            if (vkCreateFramebuffer(device_, &framebuffer_info, nullptr, &framebuffers_[i]) !=
                VK_SUCCESS) {
                throw_vk("failed to create framebuffer");
            }
        }
    }

    void create_command_pool() {
        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = *queue_indices_.graphics;
        if (vkCreateCommandPool(device_, &pool_info, nullptr, &command_pool_) != VK_SUCCESS) {
            throw_vk("failed to create command pool");
        }
    }

    void create_vertex_buffer() {
        const VkDeviceSize buffer_size = sizeof(kTriangleVertices);
        create_buffer(
            buffer_size,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertex_buffer_,
            vertex_buffer_memory_);

        void* data = nullptr;
        vkMapMemory(device_, vertex_buffer_memory_, 0, buffer_size, 0, &data);
        std::memcpy(data, kTriangleVertices.data(), static_cast<size_t>(buffer_size));
        vkUnmapMemory(device_, vertex_buffer_memory_);
    }

    void create_command_buffers() {
        command_buffers_.resize(swap_images_.size());
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = command_pool_;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers_.size());
        if (vkAllocateCommandBuffers(device_, &alloc_info, command_buffers_.data()) != VK_SUCCESS) {
            throw_vk("failed to allocate command buffers");
        }
    }

    void create_sync_objects() {
        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(device_, &semaphore_info, nullptr, &image_available_semaphore_) !=
                VK_SUCCESS ||
            vkCreateSemaphore(device_, &semaphore_info, nullptr, &render_finished_semaphore_) !=
                VK_SUCCESS ||
            vkCreateFence(device_, &fence_info, nullptr, &in_flight_fence_) != VK_SUCCESS) {
            throw_vk("failed to create sync objects");
        }
    }

    void recreate_swapchain() {
        int width = 0;
        int height = 0;
        SDL_Vulkan_GetDrawableSize(window_, &width, &height);
        while (width == 0 || height == 0) {
            SDL_WaitEvent(nullptr);
            SDL_Vulkan_GetDrawableSize(window_, &width, &height);
        }
        vkDeviceWaitIdle(device_);
        cleanup_swapchain();
        create_swapchain();
        create_image_views();
        create_render_pass();
        create_graphics_pipeline();
        create_framebuffers();
        if (menu_mode_) {
            menu_gpu_.on_render_pass_changed(render_pass_);
        }
        if (frame_pipeline_.active()) {
            frame_pipeline_.resize(swap_extent_);
        }
        menu_gpu_.on_swapchain_resize(swap_extent_);
    }

    void cleanup_swapchain() {
        for (auto framebuffer : framebuffers_) {
            vkDestroyFramebuffer(device_, framebuffer, nullptr);
        }
        framebuffers_.clear();

        if (pipeline_ != VK_NULL_HANDLE) {
            vkDestroyPipeline(device_, pipeline_, nullptr);
            pipeline_ = VK_NULL_HANDLE;
        }
        if (pipeline_layout_ != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
            pipeline_layout_ = VK_NULL_HANDLE;
        }
        if (render_pass_ != VK_NULL_HANDLE) {
            vkDestroyRenderPass(device_, render_pass_, nullptr);
            render_pass_ = VK_NULL_HANDLE;
        }

        for (auto view : swap_image_views_) {
            vkDestroyImageView(device_, view, nullptr);
        }
        swap_image_views_.clear();

        if (swapchain_ != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(device_, swapchain_, nullptr);
            swapchain_ = VK_NULL_HANDLE;
        }
    }

    VkShaderModule create_shader_module(const std::vector<char>& code) {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule module = VK_NULL_HANDLE;
        if (vkCreateShaderModule(device_, &create_info, nullptr, &module) != VK_SUCCESS) {
            throw_vk("failed to create shader module");
        }
        return module;
    }

    QueueFamilyIndices find_queue_families(VkPhysicalDevice device) {
        QueueFamilyIndices indices;
        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
        std::vector<VkQueueFamilyProperties> families(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.data());

        for (uint32_t i = 0; i < count; ++i) {
            if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics = i;
            }
            VkBool32 present_support = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &present_support);
            if (present_support) {
                indices.present = i;
            }
        }
        return indices;
    }

    bool check_device_extension_support(VkPhysicalDevice device) {
        uint32_t count = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
        std::vector<VkExtensionProperties> available(count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, available.data());

        for (const auto& ext : available) {
            if (std::strcmp(ext.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
                return true;
            }
        }
        return false;
    }

    SwapchainSupport query_swapchain_support(VkPhysicalDevice device) {
        SwapchainSupport details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);
        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, nullptr);
        if (format_count != 0) {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, details.formats.data());
        }
        uint32_t present_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_count, nullptr);
        if (present_count != 0) {
            details.present_modes.resize(present_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_count,
                                                      details.present_modes.data());
        }
        return details;
    }

    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& formats) {
        for (const auto& format : formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }
        return formats[0];
    }

    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& modes) {
        if (!config_.vsync) {
            for (const auto mode : modes) {
                if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return mode;
                }
            }
            for (const auto mode : modes) {
                if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                    return mode;
                }
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        int width = 0;
        int height = 0;
        SDL_Vulkan_GetDrawableSize(window_, &width, &height);

        VkExtent2D actual{};
        actual.width = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width,
                                  capabilities.maxImageExtent.width);
        actual.height =
            std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);
        return actual;
    }

    void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                       VkBuffer& buffer, VkDeviceMemory& memory) {
        VkBufferCreateInfo buffer_info{};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size;
        buffer_info.usage = usage;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(device_, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
            throw_vk("failed to create buffer");
        }

        VkMemoryRequirements mem_requirements;
        vkGetBufferMemoryRequirements(device_, buffer, &mem_requirements);

        VkMemoryAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = mem_requirements.size;
        alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device_, &alloc_info, nullptr, &memory) != VK_SUCCESS) {
            throw_vk("failed to allocate buffer memory");
        }
        vkBindBufferMemory(device_, buffer, memory, 0);
    }

    uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device_, &mem_properties);
        for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i) {
            if ((type_filter & (1u << i)) &&
                (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        throw_vk("failed to find suitable memory type");
        return 0;
    }

    RendererConfig config_{};
    SDL_Window* window_ = nullptr;

    VkInstance instance_ = VK_NULL_HANDLE;
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
    VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
    VkQueue graphics_queue_ = VK_NULL_HANDLE;
    VkQueue present_queue_ = VK_NULL_HANDLE;

    VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
    std::vector<VkImage> swap_images_;
    std::vector<VkImageView> swap_image_views_;
    VkFormat swap_format_ = VK_FORMAT_UNDEFINED;
    VkExtent2D swap_extent_{};

    VkRenderPass render_pass_ = VK_NULL_HANDLE;
    VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
    VkPipeline pipeline_ = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> framebuffers_;

    VkCommandPool command_pool_ = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> command_buffers_;

    VkBuffer vertex_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory vertex_buffer_memory_ = VK_NULL_HANDLE;

    VkSemaphore image_available_semaphore_ = VK_NULL_HANDLE;
    VkSemaphore render_finished_semaphore_ = VK_NULL_HANDLE;
    VkFence in_flight_fence_ = VK_NULL_HANDLE;

    QueueFamilyIndices queue_indices_{};
    uint32_t current_frame_ = 0;
    uint32_t current_image_ = 0;
    HostFramePipeline frame_pipeline_{};
    bool framebuffer_resized_ = false;
    bool should_close_ = false;
    bool skip_video_ = false;
    bool swap_image_was_presented_ = false;

    VkImage video_image_ = VK_NULL_HANDLE;
    VkDeviceMemory video_image_memory_ = VK_NULL_HANDLE;
    VkBuffer video_staging_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory video_staging_memory_ = VK_NULL_HANDLE;
    VkExtent3D video_extent_{};
    VkImageLayout video_layout_ = VK_IMAGE_LAYOUT_UNDEFINED;

    VulkanMenuGpu menu_gpu_{};
    MenuFrame menu_frame_{};
    bool menu_mode_ = false;
};

} // namespace

Renderer* create_vulkan_renderer() { return new VulkanRenderer(); }

void destroy_renderer(Renderer* renderer) {
    delete renderer;
}

} // namespace host
