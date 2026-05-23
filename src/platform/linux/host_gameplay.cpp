#include "platform/host_gameplay.h"

#include "platform/host_boot.h"
#include "platform/host_fe_assets.h"
#include "platform/host_fe_context.h"
#include "platform/host_game.h"
#include "platform/host_game_state.h"
#include "platform/host_menu_assets.h"
#include "platform/host_pad.h"
#include "platform/host_renderer.h"
#include "platform/host_ssxapp.h"
#include "platform/host_track_map.h"

#include <SDL.h>
#include <iostream>
#include <memory>

extern "C" {
void cSSXApp_preUpdate(void* self);
int host_app_tick(void);
void host_app_request_quit(void);
}

namespace host {

bool run_fe_menu_session(Renderer* renderer, const std::string& asset_dir);

bool run_world_gameplay_session(Renderer* renderer, void* ssx_app) {
    if (!renderer) {
        return false;
    }

    pad_init();

    int map_id = host_boot_race_map_id();
    if (!renderer->init_world_gameplay(map_id)) {
        pad_shutdown();
        return false;
    }

    auto* game = game_as_state(host_boot_active_game());
    const bool retail_race = game && game->in_race;

    std::cout << "[world]   3D gameplay — move: left stick / WASD, orbit: right stick, zoom: L1/R1\n";
    if (ssx_app) {
        std::cout << "[world]   Cross/Enter: (re)load track 0 — retail cGame_loadTrack + rider init\n";
    } else {
        std::cout << "[world]   Procedural flyover (use --boot-then-game for retail load chain)\n";
    }
    std::cout << "[world]   Triangle or Escape to exit\n";
    if (retail_race) {
        std::cout << "[world]   Retail race active — cSSXApp_preUpdate drives cGameViewMan_updateAll\n";
    }

    std::uint32_t last_ticks = SDL_GetTicks();
    while (!renderer->should_close()) {
        renderer->poll_events();
        pad_poll();

        if (pad_pressed(PadButton::Triangle)) {
            break;
        }

        if (ssx_app && (pad_pressed(PadButton::Cross) || pad_pressed(PadButton::Start))) {
            if (host_boot_start_race_load_mapped(0, 0)) {
                map_id = host_boot_race_map_id();
                renderer->shutdown_world_gameplay();
                if (!renderer->init_world_gameplay(map_id)) {
                    break;
                }
                game = game_as_state(host_boot_active_game());
                std::cout << "[world]   track load OK (map_id=" << map_id << ")\n";
            }
        }

        const std::uint32_t now = SDL_GetTicks();
        const float dt = static_cast<float>(now - last_ticks) * 0.001f;
        last_ticks = now;

        if (ssx_app) {
            cSSXApp_preUpdate(ssx_app);
        } else if (game && game->in_race) {
            host_game_tick(host_boot_active_game());
        }

        renderer->tick_world_gameplay(dt);

        if (!renderer->begin_frame()) {
            break;
        }
        renderer->end_frame();
        renderer->present();
    }

    renderer->shutdown_world_gameplay();
    pad_shutdown();
    return true;
}

bool run_native_world_session(const RendererConfig& config, bool retail_boot_init) {
    void* ssx_app = nullptr;
    if (retail_boot_init) {
        if (!run_retail_boot_init(0, nullptr)) {
            std::cerr << "[world]   retail boot init failed before 3D session\n";
            return false;
        }
        ssx_app = host_boot_ssx_app();
        if (ssx_app && host_boot_start_race_load_mapped(0, 0)) {
            std::cout << "[world]   retail startGameLoad → initload (track 0, in_race)\n";
        } else {
            std::cerr << "[world]   warning: retail race load failed; flyover only\n";
        }
    }

    std::unique_ptr<Renderer, decltype(&destroy_renderer)> renderer(create_renderer(),
                                                                   destroy_renderer);
    if (!renderer || !renderer->init(config)) {
        if (retail_boot_init) {
            run_retail_boot_shutdown();
        }
        return false;
    }
    const bool ok = run_world_gameplay_session(renderer.get(), ssx_app);
    renderer->shutdown();

    if (retail_boot_init) {
        run_retail_boot_shutdown();
    }
    return ok;
}

bool run_boot_appman_vulkan_session(void* ssx_app) {
    RendererConfig config{};
    config.title = "SSX3";
    config.width = 1280;
    config.height = 720;
    config.vsync = true;
    config.shader_dir = "obj/host/shaders";
    config.upscale = UpscaleBackend::Direct;

    std::unique_ptr<Renderer, decltype(&destroy_renderer)> renderer(create_renderer(),
                                                                   destroy_renderer);
    if (!renderer || !renderer->init(config)) {
        std::cerr << "[boot]    Vulkan renderer init failed; falling back to headless ticks\n";
        return false;
    }

    int map_id = host_boot_race_map_id();
    if (ssx_app) {
        auto* game = game_as_state(host_boot_active_game());
        if (!game || !game->in_race) {
            if (host_boot_start_race_load_mapped(0, 0)) {
                map_id = host_boot_race_map_id();
                std::cout << "[boot]    auto-loaded track 0 (retail cGame_loadTrack)\n";
            }
        }
    }
    if (!renderer->init_world_gameplay(map_id)) {
        renderer->shutdown();
        return false;
    }

    std::cout << "[boot]    cAppMan_mainLoop — Vulkan renderer (Enter=reload track, Esc=quit)\n";

    while (host_app_tick()) {
        renderer->poll_events();
        pad_poll();

        if (renderer->should_close()) {
            host_app_request_quit();
            break;
        }

        if (pad_pressed(PadButton::Triangle)) {
            host_app_request_quit();
            break;
        }

        if (ssx_app && (pad_pressed(PadButton::Cross) || pad_pressed(PadButton::Start))) {
            if (host_boot_start_race_load_mapped(0, 0)) {
                map_id = host_boot_race_map_id();
                renderer->shutdown_world_gameplay();
                if (!renderer->init_world_gameplay(map_id)) {
                    break;
                }
                std::cout << "[boot]    track load OK (map_id=" << map_id << ")\n";
            }
        }

        if (ssx_app) {
            cSSXApp_preUpdate(ssx_app);
        }

        static std::uint32_t last_ticks = SDL_GetTicks();
        const std::uint32_t now = SDL_GetTicks();
        const float dt = static_cast<float>(now - last_ticks) * 0.001f;
        last_ticks = now;
        renderer->tick_world_gameplay(dt);

        if (!renderer->begin_frame()) {
            break;
        }
        renderer->end_frame();
        renderer->present();
    }

    renderer->shutdown_world_gameplay();
    renderer->shutdown();
    return true;
}

bool host_launch_race_from_fe(Renderer* renderer, int load_mode, int track_id) {
    void* ssx_app = host_boot_ssx_app();
    if (!renderer || !ssx_app) {
        return false;
    }

    const std::string asset_dir = fe_menu_asset_dir();
    renderer->shutdown_main_menu();

    if (!host_boot_start_race_load_mapped(load_mode, track_id)) {
        std::cerr << "[fe]      race load failed mode=" << load_mode << " track=" << track_id
                  << '\n';
        if (!renderer->init_main_menu(asset_dir)) {
            return false;
        }
        return false;
    }

    std::cout << "[fe]      race load mode=" << load_mode << " track=" << track_id << " ("
              << track_map_label(track_id) << ") → 3D session\n";
    run_world_gameplay_session(renderer, ssx_app);

    if (!renderer->init_main_menu(asset_dir)) {
        std::cerr << "[fe]      failed to restore FE menu after race\n";
        return false;
    }
    fe_set_frame_context(renderer->width(), renderer->height(), 1.0f / 60.0f);
    return true;
}

bool run_boot_fe_vulkan_session(const RendererConfig& config) {
    const std::string asset_dir = fe_menu_asset_dir();
    if (!host_menu_assets_ready(asset_dir)) {
        std::cerr << "[boot]    FE assets missing — run: python3 scripts/extract_host_assets.py --disc disc\n";
        return false;
    }

    std::unique_ptr<Renderer, decltype(&destroy_renderer)> renderer(create_renderer(),
                                                                   destroy_renderer);
    if (!renderer || !renderer->init(config)) {
        return false;
    }

    const bool ok = run_fe_menu_session(renderer.get(), asset_dir);
    renderer->shutdown();
    return ok;
}

} // namespace host
