#include "platform/host_main_menu.h"

#include "platform/host_menu_layout.h"

#include <SDL.h>

#include <iostream>

namespace host {

bool run_main_menu_loop(Renderer* renderer, const std::string& asset_dir) {
    if (!renderer) {
        return false;
    }
    if (!renderer->init_main_menu(asset_dir)) {
        std::cerr << "[menu]    failed to load main menu assets from " << asset_dir << '\n';
        std::cerr << "[menu]    run: python3 scripts/extract_menu_assets.py --disc disc\n";
        return false;
    }

    reset_title_screen_state();
    std::cout << "[menu]    SSX3 title screen (640x448 layout, resizable window)\n";
    std::cout << "[menu]    Escape to quit\n";

    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last_ticks = SDL_GetPerformanceCounter();
    float menu_time = 0.0f;

    while (!renderer->should_close()) {
        const Uint64 now = SDL_GetPerformanceCounter();
        const float dt = static_cast<float>(now - last_ticks) / static_cast<float>(freq);
        last_ticks = now;
        menu_time += dt;

        MenuFrame frame{};
        const MenuViewport viewport =
            compute_title_viewport(renderer->width(), renderer->height());
        build_title_screen(frame, viewport, menu_time, dt);
        renderer->set_main_menu_frame(frame);

        if (!renderer->begin_frame()) {
            break;
        }
        renderer->end_frame();
        renderer->present();
    }

    renderer->shutdown_main_menu();
    reset_title_screen_state();
    return true;
}

} // namespace host
