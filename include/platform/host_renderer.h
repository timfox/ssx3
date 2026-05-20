#ifndef PLATFORM_HOST_RENDERER_H
#define PLATFORM_HOST_RENDERER_H

#include "platform/host_menu_frame.h"
#include "platform/host_upscale.h"

#include <cstdint>
#include <string>

namespace host {

struct VideoFrame;

struct RendererConfig {
    const char* title = "SSX3";
    int width = 1280;
    int height = 720;
    bool vsync = true;
    const char* shader_dir = "obj/host/shaders";
    UpscaleBackend upscale = UpscaleBackend::Compute;
    float render_scale = 0.67f;
};

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual bool init(const RendererConfig& config) = 0;
    virtual void shutdown() = 0;

    virtual bool begin_frame() = 0;
    virtual void end_frame() = 0;
    virtual void present() = 0;

    virtual bool should_close() const = 0;
    virtual void poll_events() = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual bool present_video_frame(const VideoFrame& frame) { return false; }
    virtual bool consume_skip_video() { return false; }

    virtual bool init_main_menu(const std::string& asset_dir) {
        (void)asset_dir;
        return false;
    }
    virtual void set_main_menu_frame(const MenuFrame& frame) { (void)frame; }
    virtual void shutdown_main_menu() {}
};

Renderer* create_vulkan_renderer();
void destroy_renderer(Renderer* renderer);

} // namespace host

#endif /* PLATFORM_HOST_RENDERER_H */
