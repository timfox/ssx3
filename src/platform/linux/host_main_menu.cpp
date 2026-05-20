#include "platform/host_main_menu.h"

#include "platform/host_menu_frame.h"

#define STB_EASY_FONT_IMPLEMENTATION
#include "third_party/stb_easy_font.h"

#include <SDL.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

namespace host {
namespace {

constexpr int kSnowflakeCount = 140;
constexpr int kTexBackground = 1;
constexpr int kTexLogo = 0;
constexpr int kTexSnow = 2;

struct Snowflake {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float size = 4.0f;
    float spin = 0.0f;
    float spin_v = 0.0f;
    float alpha = 0.8f;
};

struct MainMenuState {
    std::array<Snowflake, kSnowflakeCount> flakes{};
    bool flakes_ready = false;
    float menu_time = 0.0f;
};

MainMenuState g_menu;

void init_snowflakes(int width, int height) {
    std::mt19937 rng(0x53535833u);
    std::uniform_real_distribution<float> x_dist(0.0f, static_cast<float>(width));
    std::uniform_real_distribution<float> y_dist(0.0f, static_cast<float>(height));
    std::uniform_real_distribution<float> vx_dist(-18.0f, 18.0f);
    std::uniform_real_distribution<float> vy_dist(28.0f, 92.0f);
    std::uniform_real_distribution<float> size_dist(3.0f, 11.0f);
    std::uniform_real_distribution<float> alpha_dist(0.35f, 0.95f);

    for (auto& flake : g_menu.flakes) {
        flake.x = x_dist(rng);
        flake.y = y_dist(rng);
        flake.vx = vx_dist(rng);
        flake.vy = vy_dist(rng);
        flake.size = size_dist(rng);
        flake.spin = 0.0f;
        flake.spin_v = vx_dist(rng) * 0.05f;
        flake.alpha = alpha_dist(rng);
    }
    g_menu.flakes_ready = true;
}

void update_snowflakes(float dt, int width, int height) {
    if (!g_menu.flakes_ready) {
        init_snowflakes(width, height);
    }
    const float w = static_cast<float>(width);
    const float h = static_cast<float>(height);
    for (auto& flake : g_menu.flakes) {
        flake.x += flake.vx * dt;
        flake.y += flake.vy * dt;
        flake.spin += flake.spin_v * dt;
        if (flake.y > h + 16.0f) {
            flake.y = -flake.size;
            flake.x = std::fmod(flake.x + w * 0.35f + w, w);
        }
        if (flake.x < -16.0f) {
            flake.x = w + 8.0f;
        } else if (flake.x > w + 16.0f) {
            flake.x = -8.0f;
        }
    }
}

void append_sprite(std::vector<MenuSprite>& sprites, const MenuSprite& sprite) {
    sprites.push_back(sprite);
}

void append_text(std::vector<MenuSprite>& sprites,
                 const char* text,
                 float x,
                 float y,
                 float scale,
                 float r,
                 float g,
                 float b,
                 float a) {
    alignas(16) char vertex_buffer[65536];
    const int quads =
        stb_easy_font_print(0.0f, 0.0f, const_cast<char*>(text), nullptr, vertex_buffer, sizeof(vertex_buffer));
    const int num_quads = quads / 64;
    const auto* bytes = reinterpret_cast<const unsigned char*>(vertex_buffer);
    for (int i = 0; i < num_quads; ++i) {
        const unsigned char* q = bytes + i * 64;
        float min_x = 0.0f;
        float min_y = 0.0f;
        float max_x = 0.0f;
        float max_y = 0.0f;
        for (int corner = 0; corner < 4; ++corner) {
            const unsigned char* v = q + corner * 16;
            float vx = 0.0f;
            float vy = 0.0f;
            std::memcpy(&vx, v, sizeof(float));
            std::memcpy(&vy, v + 4, sizeof(float));
            if (corner == 0) {
                min_x = max_x = vx;
                min_y = max_y = vy;
            } else {
                min_x = std::min(min_x, vx);
                min_y = std::min(min_y, vy);
                max_x = std::max(max_x, vx);
                max_y = std::max(max_y, vy);
            }
        }
        MenuSprite sprite{};
        sprite.x = x + min_x * scale;
        sprite.y = y + min_y * scale;
        sprite.w = (max_x - min_x) * scale;
        sprite.h = (max_y - min_y) * scale;
        sprite.r = r;
        sprite.g = g;
        sprite.b = b;
        sprite.a = a;
        sprite.texture_index = kTexSnow;
        if (sprite.w > 0.5f && sprite.h > 0.5f) {
            append_sprite(sprites, sprite);
        }
    }
}

MenuFrame build_menu_frame(Renderer* renderer, float time_sec, float dt) {
    MenuFrame frame{};
    frame.time_sec = time_sec;
    frame.width = renderer->width();
    frame.height = renderer->height();
    if (frame.width <= 0 || frame.height <= 0) {
        return frame;
    }

    update_snowflakes(dt, frame.width, frame.height);

    const float w = static_cast<float>(frame.width);
    const float h = static_cast<float>(frame.height);

    MenuSprite bg{};
    bg.texture_index = kTexBackground;
    bg.x = 0.0f;
    bg.y = 0.0f;
    bg.w = w;
    bg.h = h;
    bg.r = 0.55f;
    bg.g = 0.62f;
    bg.b = 0.85f;
    bg.a = 0.92f;
    append_sprite(frame.sprites, bg);

    for (const Snowflake& flake : g_menu.flakes) {
        MenuSprite snow{};
        snow.texture_index = kTexSnow;
        snow.x = flake.x;
        snow.y = flake.y;
        snow.w = flake.size;
        snow.h = flake.size;
        snow.a = flake.alpha;
        append_sprite(frame.sprites, snow);
    }

    const float logo_w = std::min(w * 0.72f, 640.0f);
    const float logo_h = logo_w;
    MenuSprite logo{};
    logo.texture_index = kTexLogo;
    logo.x = (w - logo_w) * 0.5f;
    logo.y = h * 0.22f;
    logo.w = logo_w;
    logo.h = logo_h;
    logo.a = 1.0f;
    append_sprite(frame.sprites, logo);

    const float blink = 0.55f + 0.45f * std::sin(time_sec * 3.2f);
    const char* press_text = "PRESS <START> TO PLAY";
    float text_w = 0.0f;
    alignas(16) char measure_buf[4096];
    const int measure_quads =
        stb_easy_font_print(0.0f, 0.0f, const_cast<char*>(press_text), nullptr, measure_buf, sizeof(measure_buf));
    const auto* mbytes = reinterpret_cast<const unsigned char*>(measure_buf);
    const int mcount = measure_quads / 64;
    if (mcount > 0) {
        float min_x = 0.0f;
        float max_x = 0.0f;
        for (int i = 0; i < mcount; ++i) {
            const unsigned char* q = mbytes + i * 64;
            for (int corner = 0; corner < 4; ++corner) {
                const unsigned char* v = q + corner * 16;
                float vx = 0.0f;
                std::memcpy(&vx, v, sizeof(float));
                if (i == 0 && corner == 0) {
                    min_x = max_x = vx;
                } else {
                    min_x = std::min(min_x, vx);
                    max_x = std::max(max_x, vx);
                }
            }
        }
        text_w = (max_x - min_x) * 2.4f;
    }
    const float text_scale = 2.4f;
    const float text_x = std::max(0.0f, (w - text_w) * 0.5f);
    const float text_y = h * 0.82f;
    append_text(frame.sprites, press_text, text_x, text_y, text_scale, 1.0f, 1.0f, 1.0f, blink);

    return frame;
}

} // namespace

bool run_main_menu_loop(Renderer* renderer, const std::string& asset_dir) {
    if (!renderer) {
        return false;
    }
    if (!renderer->init_main_menu(asset_dir)) {
        std::cerr << "[menu]    failed to load main menu assets from " << asset_dir << '\n';
        return false;
    }

    g_menu = MainMenuState{};
    std::cout << "[menu]    SSX3 title screen (logo, snowflakes, press start)\n";
    std::cout << "[menu]    Escape to quit\n";

    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last_ticks = SDL_GetPerformanceCounter();

    while (!renderer->should_close()) {
        const Uint64 now = SDL_GetPerformanceCounter();
        const float dt = static_cast<float>(now - last_ticks) / static_cast<float>(freq);
        last_ticks = now;
        g_menu.menu_time += dt;

        renderer->set_main_menu_frame(build_menu_frame(renderer, g_menu.menu_time, dt));

        if (!renderer->begin_frame()) {
            break;
        }
        renderer->end_frame();
        renderer->present();
    }

    renderer->shutdown_main_menu();
    return true;
}

} // namespace host
