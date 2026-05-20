#include "platform/host_menu_layout.h"

#define STB_EASY_FONT_IMPLEMENTATION
#include "third_party/stb_easy_font.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <random>

namespace host {
namespace {

constexpr int kAmbientSnowCount = 85;
constexpr int kBurstSnowCount = 50;
constexpr int kTexLogo = 0;
constexpr int kTexMountain = 1;
constexpr int kTexSnow = 2;

// Retail title_logo crop is 512x276 (ui/fe_1.ssh ssxt, top band).
constexpr float kLogoAspect = 512.0f / 276.0f;

struct Snowflake {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float size = 4.0f;
    float alpha = 0.8f;
    bool burst = false;
};

struct TitleState {
    std::array<Snowflake, kAmbientSnowCount> ambient{};
    std::array<Snowflake, kBurstSnowCount> burst{};
    bool ready = false;
    int last_vw = 0;
    int last_vh = 0;
};

TitleState g_title;

void append_sprite(std::vector<MenuSprite>& sprites, const MenuSprite& sprite) {
    sprites.push_back(sprite);
}

float vpx(const MenuViewport& vp, float vx) {
    return vp.x + (vx / static_cast<float>(kTitleVirtualWidth)) * vp.width;
}

float vpy(const MenuViewport& vp, float vy) {
    return vp.y + (vy / static_cast<float>(kTitleVirtualHeight)) * vp.height;
}

float vpw(const MenuViewport& vp, float vw) {
    return (vw / static_cast<float>(kTitleVirtualWidth)) * vp.width;
}

float vph(const MenuViewport& vp, float vh) {
    return (vh / static_cast<float>(kTitleVirtualHeight)) * vp.height;
}

void append_text(std::vector<MenuSprite>& sprites,
                 const MenuViewport& vp,
                 const char* text,
                 float vx,
                 float vy,
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
    const float pixel_scale = vpw(vp, scale);
    for (int i = 0; i < num_quads; ++i) {
        const unsigned char* q = bytes + i * 64;
        float min_x = 0.0f;
        float min_y = 0.0f;
        float max_x = 0.0f;
        float max_y = 0.0f;
        for (int corner = 0; corner < 4; ++corner) {
            const unsigned char* v = q + corner * 16;
            float fx = 0.0f;
            float fy = 0.0f;
            std::memcpy(&fx, v, sizeof(float));
            std::memcpy(&fy, v + 4, sizeof(float));
            if (corner == 0) {
                min_x = max_x = fx;
                min_y = max_y = fy;
            } else {
                min_x = std::min(min_x, fx);
                min_y = std::min(min_y, fy);
                max_x = std::max(max_x, fx);
                max_y = std::max(max_y, fy);
            }
        }
        MenuSprite sprite{};
        sprite.x = vpx(vp, vx + min_x * scale);
        sprite.y = vpy(vp, vy + min_y * scale);
        sprite.w = (max_x - min_x) * pixel_scale;
        sprite.h = (max_y - min_y) * pixel_scale;
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

float measure_text_width_virtual(const char* text, float scale) {
    alignas(16) char buf[4096];
    const int quads =
        stb_easy_font_print(0.0f, 0.0f, const_cast<char*>(text), nullptr, buf, sizeof(buf));
    const auto* bytes = reinterpret_cast<const unsigned char*>(buf);
    const int count = quads / 64;
    if (count <= 0) {
        return 0.0f;
    }
    float min_x = 0.0f;
    float max_x = 0.0f;
    for (int i = 0; i < count; ++i) {
        const unsigned char* q = bytes + i * 64;
        for (int corner = 0; corner < 4; ++corner) {
            float vx = 0.0f;
            std::memcpy(&vx, q + corner * 16, sizeof(float));
            if (i == 0 && corner == 0) {
                min_x = max_x = vx;
            } else {
                min_x = std::min(min_x, vx);
                max_x = std::max(max_x, vx);
            }
        }
    }
    return (max_x - min_x) * scale;
}

void init_snow(float logo_cx, float logo_cy, float logo_w) {
    std::mt19937 rng(0x53535833u);
    std::uniform_real_distribution<float> x_dist(0.0f, static_cast<float>(kTitleVirtualWidth));
    std::uniform_real_distribution<float> y_dist(0.0f, static_cast<float>(kTitleVirtualHeight));
    std::uniform_real_distribution<float> vx_amb(-10.0f, 10.0f);
    // Positive virtual vy moves up on screen with our NDC mapping; use negative for fall.
    std::uniform_real_distribution<float> vy_amb(-32.0f, -8.0f);
    std::uniform_real_distribution<float> size_amb(1.5f, 4.5f);
    std::uniform_real_distribution<float> alpha_amb(0.2f, 0.55f);

    for (auto& flake : g_title.ambient) {
        flake.x = x_dist(rng);
        flake.y = y_dist(rng);
        flake.vx = vx_amb(rng);
        flake.vy = vy_amb(rng);
        flake.size = size_amb(rng);
        flake.alpha = alpha_amb(rng);
        flake.burst = false;
    }

    std::normal_distribution<float> burst_x(logo_cx + logo_w * 0.14f, logo_w * 0.09f);
    std::normal_distribution<float> burst_y(logo_cy - logo_w * 0.02f, logo_w * 0.05f);
    std::uniform_real_distribution<float> burst_vx(-20.0f, 70.0f);
    std::uniform_real_distribution<float> burst_vy(-48.0f, -12.0f);
    std::uniform_real_distribution<float> size_burst(2.0f, 7.0f);
    std::uniform_real_distribution<float> alpha_burst(0.5f, 1.0f);

    for (auto& flake : g_title.burst) {
        flake.x = burst_x(rng);
        flake.y = burst_y(rng);
        flake.vx = burst_vx(rng);
        flake.vy = burst_vy(rng);
        flake.size = size_burst(rng);
        flake.alpha = alpha_burst(rng);
        flake.burst = true;
    }
    g_title.ready = true;
}

void update_snow(float dt, float logo_cx, float logo_cy, float logo_w) {
    if (!g_title.ready) {
        init_snow(logo_cx, logo_cy, logo_w);
    }

    const float w = static_cast<float>(kTitleVirtualWidth);
    const float h = static_cast<float>(kTitleVirtualHeight);

    auto step = [&](Snowflake& flake) {
        flake.x += flake.vx * dt;
        flake.y += flake.vy * dt;
        if (flake.burst) {
            flake.vy -= 6.0f * dt;
            flake.vx *= 1.0f - 0.25f * dt;
            if (flake.y < logo_cy - logo_w * 0.35f || flake.x > w + 12.0f) {
                std::mt19937 rng(static_cast<uint32_t>(flake.x * 19.0f + flake.y * 7.0f));
                std::normal_distribution<float> burst_x(logo_cx + logo_w * 0.14f, logo_w * 0.08f);
                std::normal_distribution<float> burst_y(logo_cy - logo_w * 0.06f, logo_w * 0.04f);
                flake.x = burst_x(rng);
                flake.y = burst_y(rng);
                flake.vx = 18.0f + static_cast<float>(rng() % 55);
                flake.vy = -14.0f - static_cast<float>(rng() % 28);
            }
        } else {
            if (flake.y < -flake.size - 8.0f) {
                flake.y = h + flake.size;
                flake.x = std::fmod(flake.x + w * 0.15f + w, w);
            }
            if (flake.x < -8.0f) {
                flake.x = w + 4.0f;
            } else if (flake.x > w + 8.0f) {
                flake.x = -4.0f;
            }
        }
    };

    for (auto& flake : g_title.ambient) {
        step(flake);
    }
    for (auto& flake : g_title.burst) {
        step(flake);
    }
}

} // namespace

MenuViewport compute_title_viewport(int window_width, int window_height) {
    MenuViewport vp{};
    vp.window_width = std::max(window_width, 1);
    vp.window_height = std::max(window_height, 1);

    const float target_aspect =
        static_cast<float>(kTitleVirtualWidth) / static_cast<float>(kTitleVirtualHeight);
    const float window_aspect =
        static_cast<float>(vp.window_width) / static_cast<float>(vp.window_height);

    if (window_aspect > target_aspect) {
        vp.height = static_cast<float>(vp.window_height);
        vp.width = vp.height * target_aspect;
        vp.x = (static_cast<float>(vp.window_width) - vp.width) * 0.5f;
        vp.y = 0.0f;
    } else {
        vp.width = static_cast<float>(vp.window_width);
        vp.height = vp.width / target_aspect;
        vp.x = 0.0f;
        vp.y = (static_cast<float>(vp.window_height) - vp.height) * 0.5f;
    }
    return vp;
}

void reset_title_screen_state() {
    g_title = TitleState{};
}

void build_title_screen(MenuFrame& frame,
                        const MenuViewport& viewport,
                        float time_sec,
                        float dt,
                        const MenuRetailStyle& style) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f) {
        return;
    }

    if (g_title.last_vw != static_cast<int>(viewport.width) ||
        g_title.last_vh != static_cast<int>(viewport.height)) {
        g_title.ready = false;
        g_title.last_vw = static_cast<int>(viewport.width);
        g_title.last_vh = static_cast<int>(viewport.height);
    }

    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    // Retail layout (640x448): wide icy logo, mountains left, heavy bottom fog.
    const float logo_w = 548.0f;
    const float logo_h = logo_w / kLogoAspect;
    const float logo_x = (vw - logo_w) * 0.5f;
    const float logo_y = 86.0f;
    const float logo_cx = logo_x + logo_w * 0.5f;
    const float logo_cy = logo_y + logo_h * 0.5f;

    update_snow(dt, logo_cx, logo_cy, logo_w);

    // Sky gradient
    MenuSprite sky{};
    sky.texture_index = kTexSnow;
    sky.x = vpx(viewport, 0.0f);
    sky.y = vpy(viewport, 0.0f);
    sky.w = vpw(viewport, vw);
    sky.h = vph(viewport, vh);
    sky.gradient_y = true;
    sky.r = 0.70f;
    sky.g = 0.82f;
    sky.b = 0.95f;
    sky.a = 1.0f;
    sky.r2 = 0.96f;
    sky.g2 = 0.98f;
    sky.b2 = 1.0f;
    sky.a2 = 1.0f;
    append_sprite(frame.sprites, sky);

    // Distant peaks (left-heavy, matches retail mountain placement)
    MenuSprite mountain{};
    mountain.texture_index = kTexMountain;
    mountain.x = vpx(viewport, -48.0f);
    mountain.y = vpy(viewport, 118.0f);
    mountain.w = vpw(viewport, 390.0f);
    mountain.h = vph(viewport, 250.0f);
    mountain.r = 0.58f;
    mountain.g = 0.70f;
    mountain.b = 0.84f;
    mountain.a = 0.82f;
    append_sprite(frame.sprites, mountain);

    MenuSprite mountain2 = mountain;
    mountain2.x = vpx(viewport, 168.0f);
    mountain2.y = vpy(viewport, 152.0f);
    mountain2.w = vpw(viewport, 300.0f);
    mountain2.h = vph(viewport, 190.0f);
    mountain2.a = 0.48f;
    append_sprite(frame.sprites, mountain2);

    // Bottom whiteout fog overlay
    MenuSprite fog{};
    fog.texture_index = kTexSnow;
    fog.x = vpx(viewport, 0.0f);
    fog.y = vpy(viewport, 250.0f);
    fog.w = vpw(viewport, vw);
    fog.h = vph(viewport, vh - 250.0f);
    fog.gradient_y = true;
    fog.r = 1.0f;
    fog.g = 1.0f;
    fog.b = 1.0f;
    fog.a = 0.0f;
    fog.r2 = 1.0f;
    fog.g2 = 1.0f;
    fog.b2 = 1.0f;
    fog.a2 = 0.92f;
    append_sprite(frame.sprites, fog);

    for (const Snowflake& flake : g_title.ambient) {
        MenuSprite snow{};
        snow.texture_index = kTexSnow;
        snow.x = vpx(viewport, flake.x);
        snow.y = vpy(viewport, flake.y);
        snow.w = vpw(viewport, flake.size);
        snow.h = vph(viewport, flake.size);
        snow.r = style.snow_r;
        snow.g = style.snow_g;
        snow.b = style.snow_b;
        snow.a = flake.alpha;
        append_sprite(frame.sprites, snow);
    }

    MenuSprite logo{};
    logo.texture_index = kTexLogo;
    logo.x = vpx(viewport, logo_x);
    logo.y = vpy(viewport, logo_y);
    logo.w = vpw(viewport, logo_w);
    logo.h = vph(viewport, logo_h);
    logo.a = 1.0f;
    append_sprite(frame.sprites, logo);

    for (const Snowflake& flake : g_title.burst) {
        MenuSprite snow{};
        snow.texture_index = kTexSnow;
        snow.x = vpx(viewport, flake.x);
        snow.y = vpy(viewport, flake.y);
        snow.w = vpw(viewport, flake.size);
        snow.h = vph(viewport, flake.size);
        snow.r = style.snow_r;
        snow.g = style.snow_g;
        snow.b = style.snow_b;
        snow.a = flake.alpha;
        append_sprite(frame.sprites, snow);
    }

    const float blink = 0.4f + 0.6f * (0.5f + 0.5f * std::sin(time_sec * 3.0f));
    const char* press_text = "press <start> to play";
    const float press_scale = 2.05f;
    const float press_w = measure_text_width_virtual(press_text, press_scale);
    append_text(frame.sprites,
                viewport,
                press_text,
                (vw - press_w) * 0.5f,
                302.0f,
                press_scale,
                0.10f,
                0.11f,
                0.13f,
                blink);

    const char* copy_text = "Copyright Electronic Arts";
    const float copy_scale = 1.15f;
    const float copy_w = measure_text_width_virtual(copy_text, copy_scale);
    append_text(frame.sprites,
                viewport,
                copy_text,
                (vw - copy_w) * 0.5f,
                412.0f,
                copy_scale,
                0.07f,
                0.07f,
                0.09f,
                0.95f);
}

void build_main_menu_screen(MenuFrame& frame,
                            const MenuViewport& viewport,
                            float time_sec,
                            int selected_item,
                            int item_count) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f || item_count <= 0) {
        return;
    }

    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    MenuSprite sky{};
    sky.texture_index = kTexSnow;
    sky.x = vpx(viewport, 0.0f);
    sky.y = vpy(viewport, 0.0f);
    sky.w = vpw(viewport, vw);
    sky.h = vph(viewport, vh);
    sky.gradient_y = true;
    sky.r = 0.55f;
    sky.g = 0.68f;
    sky.b = 0.82f;
    sky.a = 1.0f;
    sky.r2 = 0.88f;
    sky.g2 = 0.92f;
    sky.b2 = 0.98f;
    sky.a2 = 1.0f;
    append_sprite(frame.sprites, sky);

    MenuSprite mountain{};
    mountain.texture_index = kTexMountain;
    mountain.x = vpx(viewport, -60.0f);
    mountain.y = vpy(viewport, 80.0f);
    mountain.w = vpw(viewport, 420.0f);
    mountain.h = vph(viewport, 320.0f);
    mountain.r = 0.65f;
    mountain.g = 0.78f;
    mountain.b = 0.90f;
    mountain.a = 0.95f;
    append_sprite(frame.sprites, mountain);

    const float logo_w = 320.0f;
    const float logo_h = logo_w / kLogoAspect;
    MenuSprite logo{};
    logo.texture_index = kTexLogo;
    logo.x = vpx(viewport, (vw - logo_w) * 0.5f);
    logo.y = vpy(viewport, 24.0f);
    logo.w = vpw(viewport, logo_w);
    logo.h = vph(viewport, logo_h);
    logo.a = 0.92f;
    append_sprite(frame.sprites, logo);

    static const char* kMenuItems[] = {
        "Freeride",
        "World Tour",
        "Multiplayer",
        "Options",
        "Profile",
    };
    const int count = static_cast<int>(sizeof(kMenuItems) / sizeof(kMenuItems[0]));
    const int items = item_count < count ? item_count : count;
    const float base_x = 368.0f;
    const float base_y = 168.0f;
    const float line_h = 38.0f;
    const float base_scale = 2.35f;
    const float sel_scale = 2.75f;

    for (int i = 0; i < items; ++i) {
        const bool selected = (i == selected_item);
        const float scale = selected ? sel_scale : base_scale;
        const float pulse = selected ? (0.85f + 0.15f * std::sin(time_sec * 5.0f)) : 1.0f;
        append_text(frame.sprites,
                    viewport,
                    kMenuItems[i],
                    base_x,
                    base_y + static_cast<float>(i) * line_h,
                    scale,
                    selected ? 0.05f : 0.12f,
                    selected ? 0.08f : 0.14f,
                    selected ? 0.12f : 0.18f,
                    selected ? pulse : 0.82f);
    }

    append_text(frame.sprites,
                viewport,
                "D-Pad / arrows  ·  Cross confirm  ·  Circle back",
                48.0f,
                418.0f,
                1.35f,
                0.35f,
                0.38f,
                0.42f,
                0.75f);
}

void build_mountain_room_screen(MenuFrame& frame,
                                const MenuViewport& viewport,
                                float time_sec,
                                int selected_peak,
                                int peak_count) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f || peak_count <= 0) {
        return;
    }

    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    MenuSprite sky{};
    sky.texture_index = kTexSnow;
    sky.x = vpx(viewport, 0.0f);
    sky.y = vpy(viewport, 0.0f);
    sky.w = vpw(viewport, vw);
    sky.h = vph(viewport, vh);
    sky.gradient_y = true;
    sky.r = 0.18f;
    sky.g = 0.28f;
    sky.b = 0.42f;
    sky.a = 1.0f;
    sky.r2 = 0.42f;
    sky.g2 = 0.52f;
    sky.b2 = 0.68f;
    sky.a2 = 1.0f;
    append_sprite(frame.sprites, sky);

    const float pan = 0.5f + 0.12f * std::sin(time_sec * 0.35f);
    MenuSprite panorama{};
    panorama.texture_index = kTexMountain;
    panorama.x = vpx(viewport, -24.0f);
    panorama.y = vpy(viewport, 40.0f);
    panorama.w = vpw(viewport, vw + 48.0f);
    panorama.h = vph(viewport, 340.0f);
    panorama.u0 = pan - 0.35f;
    panorama.u1 = pan + 0.35f;
    panorama.v0 = 0.0f;
    panorama.v1 = 1.0f;
    panorama.a = 1.0f;
    append_sprite(frame.sprites, panorama);

    MenuSprite vignette{};
    vignette.texture_index = kTexSnow;
    vignette.x = vpx(viewport, 0.0f);
    vignette.y = vpy(viewport, 0.0f);
    vignette.w = vpw(viewport, vw);
    vignette.h = vph(viewport, vh);
    vignette.gradient_y = true;
    vignette.r = 0.0f;
    vignette.g = 0.0f;
    vignette.b = 0.0f;
    vignette.a = 0.35f;
    vignette.r2 = 0.0f;
    vignette.g2 = 0.0f;
    vignette.b2 = 0.0f;
    vignette.a2 = 0.65f;
    append_sprite(frame.sprites, vignette);

    static const char* kPeaks[] = {"Peak A", "Peak B", "Peak C"};
    const int count = static_cast<int>(sizeof(kPeaks) / sizeof(kPeaks[0]));
    const int peaks = peak_count < count ? peak_count : count;
    const float card_w = 168.0f;
    const float card_y = 318.0f;
    const float gap = 12.0f;
    const float total_w = static_cast<float>(peaks) * card_w + static_cast<float>(peaks - 1) * gap;
    float card_x = (vw - total_w) * 0.5f;

    for (int i = 0; i < peaks; ++i) {
        const bool selected = (i == selected_peak);
        const float scale = selected ? 2.1f : 1.75f;
        const float pulse = selected ? (0.9f + 0.1f * std::sin(time_sec * 6.0f)) : 0.75f;
        append_text(frame.sprites,
                    viewport,
                    kPeaks[i],
                    card_x,
                    card_y,
                    scale,
                    selected ? 0.95f : 0.55f,
                    selected ? 0.97f : 0.60f,
                    selected ? 1.0f : 0.65f,
                    pulse);
        card_x += card_w + gap;
    }

    append_text(frame.sprites,
                viewport,
                "Freeride — mountain room (retail cFEStateMountainRoom)",
                28.0f,
                28.0f,
                1.45f,
                0.75f,
                0.80f,
                0.88f,
                0.9f);

    append_text(frame.sprites,
                viewport,
                "Left/Right peak · Cross race · Circle back",
                36.0f,
                418.0f,
                1.3f,
                0.4f,
                0.42f,
                0.48f,
                0.8f);
}

void build_options_screen(MenuFrame& frame,
                          const MenuViewport& viewport,
                          float time_sec,
                          int selected_item,
                          int item_count) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f || item_count <= 0) {
        return;
    }

    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    MenuSprite backdrop{};
    backdrop.texture_index = kTexSnow;
    backdrop.x = vpx(viewport, 0.0f);
    backdrop.y = vpy(viewport, 0.0f);
    backdrop.w = vpw(viewport, vw);
    backdrop.h = vph(viewport, vh);
    backdrop.gradient_y = true;
    backdrop.r = 0.08f;
    backdrop.g = 0.10f;
    backdrop.b = 0.14f;
    backdrop.a = 1.0f;
    backdrop.r2 = 0.20f;
    backdrop.g2 = 0.22f;
    backdrop.b2 = 0.28f;
    backdrop.a2 = 1.0f;
    append_sprite(frame.sprites, backdrop);

    append_text(frame.sprites,
                viewport,
                "Options",
                48.0f,
                40.0f,
                3.2f,
                0.92f,
                0.94f,
                0.98f,
                1.0f);

    static const char* kItems[] = {
        "Game",
        "Sound",
        "Controller",
        "Save / Load",
        "Back",
    };
    const int count = static_cast<int>(sizeof(kItems) / sizeof(kItems[0]));
    const int items = item_count < count ? item_count : count;
    const float base_x = 72.0f;
    const float base_y = 120.0f;
    const float line_h = 42.0f;

    for (int i = 0; i < items; ++i) {
        const bool selected = (i == selected_item);
        const float scale = selected ? 2.5f : 2.1f;
        append_text(frame.sprites,
                    viewport,
                    kItems[i],
                    base_x,
                    base_y + static_cast<float>(i) * line_h,
                    scale,
                    selected ? 0.08f : 0.45f,
                    selected ? 0.10f : 0.48f,
                    selected ? 0.14f : 0.52f,
                    selected ? 1.0f : 0.78f);
    }

    (void)time_sec;
}

void build_options_game_screen(MenuFrame& frame,
                               const MenuViewport& viewport,
                               float time_sec,
                               int selected_row,
                               int row_count) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f || row_count <= 0) {
        return;
    }

    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    MenuSprite backdrop{};
    backdrop.texture_index = kTexSnow;
    backdrop.x = vpx(viewport, 0.0f);
    backdrop.y = vpy(viewport, 0.0f);
    backdrop.w = vpw(viewport, vw);
    backdrop.h = vph(viewport, vh);
    backdrop.gradient_y = true;
    backdrop.r = 0.06f;
    backdrop.g = 0.08f;
    backdrop.b = 0.12f;
    backdrop.a = 1.0f;
    backdrop.r2 = 0.16f;
    backdrop.g2 = 0.18f;
    backdrop.b2 = 0.24f;
    backdrop.a2 = 1.0f;
    append_sprite(frame.sprites, backdrop);

    append_text(frame.sprites,
                viewport,
                "Game Options",
                48.0f,
                36.0f,
                2.8f,
                0.90f,
                0.92f,
                0.96f,
                1.0f);

    static const char* kRows[] = {
        "Difficulty: Medium",
        "Camera: Dynamic",
        "Trick speed: Normal",
        "Crash resistance: On",
        "Back",
    };
    const int count = static_cast<int>(sizeof(kRows) / sizeof(kRows[0]));
    const int rows = row_count < count ? row_count : count;
    const float base_x = 56.0f;
    const float base_y = 108.0f;
    const float line_h = 36.0f;

    for (int i = 0; i < rows; ++i) {
        const bool selected = (i == selected_row);
        append_text(frame.sprites,
                    viewport,
                    kRows[i],
                    base_x,
                    base_y + static_cast<float>(i) * line_h,
                    selected ? 2.2f : 1.85f,
                    selected ? 0.10f : 0.50f,
                    selected ? 0.12f : 0.52f,
                    selected ? 0.16f : 0.56f,
                    selected ? 1.0f : 0.80f);
    }

    append_text(frame.sprites,
                viewport,
                "Left/Right change value (stub) · Circle/Esc back",
                40.0f,
                412.0f,
                1.25f,
                0.40f,
                0.42f,
                0.48f,
                0.72f);
}

} // namespace host
