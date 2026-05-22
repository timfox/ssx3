#include "platform/host_gfx.h"

#include "platform/host_fe_assets.h"

#include "platform/host_boot_sequence.h"
#include "platform/host_main_menu.h"
#include "platform/host_renderer.h"

#include <iostream>
#include <memory>

namespace host {

bool run_vulkan_demo(const RendererConfig& config) {
    return run_native_gfx_session(config, {}, false);
}

bool run_native_gfx_session(const RendererConfig& config,
                            const std::string& disc_root,
                            bool boot_videos,
                            bool run_demo_after) {
    RendererConfig session = config;
    if (run_demo_after) {
        session.upscale = UpscaleBackend::Direct;
    }

    std::unique_ptr<Renderer, decltype(&destroy_renderer)> renderer(create_renderer(),
                                                                   destroy_renderer);
    if (!renderer || !renderer->init(session)) {
        return false;
    }

    if (boot_videos && !disc_root.empty()) {
        std::cout << "[video]   Space/Enter skip movie, Escape quit\n";
        if (!run_boot_video_sequence(disc_root, renderer.get())) {
            std::cout << "[video]   boot movies skipped or unavailable (see docs/VIDEO_CODECS.md)\n";
        }
        if (renderer->should_close()) {
            renderer->shutdown();
            return true;
        }
    }

    if (!run_demo_after) {
        renderer->shutdown();
        return true;
    }

    const std::string asset_dir = host::fe_menu_asset_dir();
    if (!run_main_menu_loop(renderer.get(), asset_dir)) {
        renderer->shutdown();
        return false;
    }

    renderer->shutdown();
    return true;
}

} // namespace host
