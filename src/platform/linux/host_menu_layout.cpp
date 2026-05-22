#include "platform/host_menu_layout.h"

#include "platform/host_fe_atlas.h"
#include "platform/host_vulkan_menu.h"
#include "platform/host_visualfx_menu.h"
#include "platform/host_fe_font.h"
#include "platform/host_fe_layout.h"
#include "platform/host_fe_layout_runtime.h"
#include "platform/host_vulkan_menu.h"
#include "platform/host_fe_strings.h"

#define STB_EASY_FONT_IMPLEMENTATION
#include "third_party/stb_easy_font.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <random>

namespace host {
namespace {

constexpr int kAmbientSnowCount = 85;
constexpr int kBurstSnowCount = 50;

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

void append_fullwindow_sky(std::vector<MenuSprite>& sprites,
                           const MenuViewport& viewport,
                           float r,
                           float g,
                           float b,
                           float r2,
                           float g2,
                           float b2) {
    MenuSprite sky{};
    sky.texture_index = kTexMenuSnow;
    sky.x = 0.0f;
    sky.y = 0.0f;
    sky.w = static_cast<float>(viewport.window_width);
    sky.h = static_cast<float>(viewport.window_height);
    sky.gradient_y = true;
    sky.r = r;
    sky.g = g;
    sky.b = b;
    sky.a = 1.0f;
    sky.r2 = r2;
    sky.g2 = g2;
    sky.b2 = b2;
    sky.a2 = 1.0f;
    append_sprite(sprites, sky);
}

void append_virtual_rect(std::vector<MenuSprite>& sprites,
                         const MenuViewport& vp,
                         int texture_index,
                         float vx,
                         float vy,
                         float vw,
                         float vh,
                         float alpha = 1.0f,
                         float r = 1.0f,
                         float g = 1.0f,
                         float b = 1.0f,
                         float u0 = 0.0f,
                         float v0 = 0.0f,
                         float u1 = 1.0f,
                         float v1 = 1.0f) {
    MenuSprite sprite{};
    sprite.texture_index = texture_index;
    sprite.x = vpx(vp, vx);
    sprite.y = vpy(vp, vy);
    sprite.w = vpw(vp, vw);
    sprite.h = vph(vp, vh);
    sprite.u0 = u0;
    sprite.v0 = v0;
    sprite.u1 = u1;
    sprite.v1 = v1;
    sprite.r = r;
    sprite.g = g;
    sprite.b = b;
    sprite.a = alpha;
    append_sprite(sprites, sprite);
}

void append_solid_rect(std::vector<MenuSprite>& sprites,
                       const MenuViewport& vp,
                       float vx,
                       float vy,
                       float vw,
                       float vh,
                       float r,
                       float g,
                       float b,
                       float a) {
    append_virtual_rect(sprites, vp, kTexMenuSolid, vx, vy, vw, vh, a, r, g, b);
}

void append_texture_rect(std::vector<MenuSprite>& sprites,
                         const MenuViewport& vp,
                         int texture_index,
                         float vx,
                         float vy,
                         float vw,
                         float vh,
                         const FeAtlasRect& uv,
                         float alpha = 1.0f,
                         float r = 1.0f,
                         float g = 1.0f,
                         float b = 1.0f) {
    append_virtual_rect(sprites,
                        vp,
                        texture_index,
                        vx,
                        vy,
                        vw,
                        vh,
                        alpha,
                        r,
                        g,
                        b,
                        uv.u0,
                        uv.v0,
                        uv.u1,
                        uv.v1);
}

void append_dashed_line_h(std::vector<MenuSprite>& sprites,
                          const MenuViewport& vp,
                          float x0,
                          float x1,
                          float y,
                          float thickness = 1.0f) {
    constexpr float dash = 6.0f;
    constexpr float gap = 5.0f;
    for (float x = x0; x < x1; x += dash + gap) {
        const float w = std::min(dash, x1 - x);
        append_solid_rect(sprites, vp, x, y, w, thickness, 0.45f, 0.48f, 0.52f, 0.28f);
    }
}

void append_dashed_line_v(std::vector<MenuSprite>& sprites,
                          const MenuViewport& vp,
                          float x,
                          float y0,
                          float y1,
                          float thickness = 1.0f) {
    constexpr float dash = 6.0f;
    constexpr float gap = 5.0f;
    for (float y = y0; y < y1; y += dash + gap) {
        const float h = std::min(dash, y1 - y);
        append_solid_rect(sprites, vp, x, y, thickness, h, 0.45f, 0.48f, 0.52f, 0.28f);
    }
}

void append_retail_menu_sky(std::vector<MenuSprite>& sprites, const MenuViewport& vp) {
    /* Retail main menu: light icy blue (not fe_1_EA_b — that atlas is dark grey). */
    append_fullwindow_sky(sprites, vp, 0.72f, 0.82f, 0.92f, 0.58f, 0.68f, 0.82f);
}

void append_menu_backdrop(std::vector<MenuSprite>& sprites, const MenuViewport& vp) {
    append_retail_menu_sky(sprites, vp);
}

void append_main_menu_orange_frame(std::vector<MenuSprite>& sprites, const MenuViewport& vp) {
    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);
    const float orange_r = 0.92f;
    const float orange_g = 0.42f;
    const float orange_b = 0.08f;
    append_solid_rect(sprites, vp, 0.0f, vh - 18.0f, vw, 18.0f, orange_r, orange_g, orange_b, 1.0f);
    append_solid_rect(sprites, vp, 0.0f, 0.0f, 14.0f, vh, orange_r, orange_g, orange_b, 1.0f);
}

void append_main_menu_left_panel(std::vector<MenuSprite>& sprites,
                                 const MenuViewport& vp,
                                 float column_divider_x) {
    append_solid_rect(sprites,
                      vp,
                      14.0f,
                      52.0f,
                      column_divider_x - 22.0f,
                      268.0f,
                      0.22f,
                      0.38f,
                      0.58f,
                      0.92f);
}

void append_menu_list_highlight(std::vector<MenuSprite>& sprites,
                                const MenuViewport& vp,
                                float sel_x,
                                float sel_y,
                                float sel_w,
                                float sel_h) {
    append_solid_rect(sprites, vp, sel_x, sel_y, sel_w, sel_h, 0.94f, 0.44f, 0.06f, 1.0f);
}

void append_options_backdrop(std::vector<MenuSprite>& sprites, const MenuViewport& vp) {
    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);
    append_fullwindow_sky(sprites, vp, 0.12f, 0.16f, 0.22f, 0.18f, 0.22f, 0.30f);
    append_virtual_rect(sprites,
                        vp,
                        kTexMenuHudPanel,
                        FeOptionsLayout::panel_x,
                        FeOptionsLayout::panel_y,
                        FeOptionsLayout::panel_w,
                        FeOptionsLayout::panel_h,
                        0.94f);
    append_solid_rect(sprites, vp, 0.0f, 0.0f, vw, vh, 0.0f, 0.0f, 0.0f, 0.28f);
}

void append_options_sub_backdrop(std::vector<MenuSprite>& sprites, const MenuViewport& vp) {
    append_options_backdrop(sprites, vp);
    append_virtual_rect(sprites,
                        vp,
                        kTexMenuHudPanel,
                        FeOptionsSubLayout::panel_x,
                        FeOptionsSubLayout::panel_y,
                        FeOptionsSubLayout::panel_w,
                        FeOptionsSubLayout::panel_h,
                        0.96f);
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
    if (host_fe_font_is_loaded()) {
        append_fe_text(sprites, vp, text, vx, vy, scale, r, g, b, a);
        return;
    }

    alignas(16) char vertex_buffer[65536];
    /* stb_easy_font_print returns quad count (bytes written / 64), not byte length.
     * Each quad is a solid axis-aligned rectangle (no texture); use kTexSolid. */
    const int num_quads =
        stb_easy_font_print(0.0f, 0.0f, const_cast<char*>(text), nullptr, vertex_buffer, sizeof(vertex_buffer));
    const auto* bytes = reinterpret_cast<const unsigned char*>(vertex_buffer);
    const float scale_x = vpw(vp, scale);
    const float scale_y = vph(vp, scale);
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
        const float glyph_w = (max_x - min_x) * scale_x;
        const float glyph_h = (max_y - min_y) * scale_y;
        if (glyph_w < 0.25f && glyph_h < 0.25f) {
            continue;
        }

        MenuSprite sprite{};
        sprite.x = vpx(vp, vx + min_x * scale);
        sprite.y = vpy(vp, vy + min_y * scale);
        sprite.w = std::max(glyph_w, 1.0f);
        sprite.h = std::max(glyph_h, 1.0f);
        sprite.u0 = 0.0f;
        sprite.v0 = 0.0f;
        sprite.u1 = 1.0f;
        sprite.v1 = 1.0f;
        sprite.r = r;
        sprite.g = g;
        sprite.b = b;
        sprite.a = a;
        sprite.texture_index = kTexMenuSolid;
        append_sprite(sprites, sprite);
    }
}

void append_main_menu_controller_hints(std::vector<MenuSprite>& sprites,
                                       const MenuViewport& vp,
                                       float hint_y,
                                       float hint_scale) {
    struct HintRow {
        FeAtlasRect icon;
        const char* label;
    };
    static const HintRow kRows[] = {
        {FePs2PadAtlas::kCross, fe_string_hint_select()},
        {FePs2PadAtlas::kTriangle, fe_string_hint_previous()},
        {FePs2PadAtlas::kSquare, fe_string_hint_options()},
    };

    if (!vulkan_menu_texture_ready(kTexMenuBgAccent)) {
        append_text(sprites,
                    vp,
                    fe_string_main_menu_hints(),
                    318.0f,
                    hint_y,
                    hint_scale,
                    0.08f,
                    0.08f,
                    0.10f,
                    0.92f);
        return;
    }

    constexpr float icon = 22.0f;
    constexpr float group_w = 106.0f;
    constexpr float start_x = 318.0f;
    const float text_y = hint_y + 3.0f;
    const float text_scale = hint_scale * 0.96f;

    for (int i = 0; i < 3; ++i) {
        const float gx = start_x + static_cast<float>(i) * group_w;
        append_texture_rect(sprites,
                            vp,
                            kTexMenuBgAccent,
                            gx,
                            hint_y,
                            icon,
                            icon,
                            kRows[i].icon,
                            1.0f);
        append_text(sprites,
                    vp,
                    kRows[i].label,
                    gx + icon + 5.0f,
                    text_y,
                    text_scale,
                    0.06f,
                    0.06f,
                    0.08f,
                    0.95f);
    }
}

void append_menu_list_label(std::vector<MenuSprite>& sprites,
                            const MenuViewport& vp,
                            const char* text,
                            float x,
                            float y,
                            float scale,
                            float scale_sel,
                            bool selected,
                            bool disabled) {
    const float use_scale = selected ? scale_sel : scale;
    float r = 0.05f;
    float g = 0.05f;
    float b = 0.08f;
    float a = 0.95f;
    if (selected) {
        r = g = 1.0f;
        b = 1.0f;
        a = 1.0f;
    } else if (disabled) {
        r = 0.45f;
        g = 0.48f;
        b = 0.55f;
        a = 0.75f;
    }
    append_text(sprites, vp, text, x, y, use_scale, r, g, b, a);
}

void append_options_list_label(std::vector<MenuSprite>& sprites,
                               const MenuViewport& vp,
                               const char* text,
                               float x,
                               float y,
                               float scale,
                               float scale_sel,
                               bool selected) {
    const float use_scale = selected ? scale_sel : scale;
    const float r = selected ? 0.98f : 0.62f;
    const float g = selected ? 0.98f : 0.64f;
    const float b = selected ? 1.0f : 0.68f;
    append_text(sprites, vp, text, x, y, use_scale, r, g, b, selected ? 1.0f : 0.86f);
}

float measure_text_width_virtual(const char* text, float scale) {
    if (host_fe_font_is_loaded()) {
        return measure_fe_text_width_virtual(text, scale);
    }

    alignas(16) char buf[4096];
    const int count =
        stb_easy_font_print(0.0f, 0.0f, const_cast<char*>(text), nullptr, buf, sizeof(buf));
    const auto* bytes = reinterpret_cast<const unsigned char*>(buf);
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
    // Virtual coords: y=0 at top; positive vy moves downward (falling snow).
    std::uniform_real_distribution<float> vy_amb(8.0f, 32.0f);
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
    std::uniform_real_distribution<float> burst_vy(12.0f, 48.0f);
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
            flake.vy += 6.0f * dt;
            flake.vx *= 1.0f - 0.25f * dt;
            if (flake.y < logo_cy - logo_w * 0.35f || flake.x > w + 12.0f) {
                std::mt19937 rng(static_cast<uint32_t>(flake.x * 19.0f + flake.y * 7.0f));
                std::normal_distribution<float> burst_x(logo_cx + logo_w * 0.14f, logo_w * 0.08f);
                std::normal_distribution<float> burst_y(logo_cy - logo_w * 0.06f, logo_w * 0.04f);
                flake.x = burst_x(rng);
                flake.y = burst_y(rng);
                flake.vx = 18.0f + static_cast<float>(rng() % 55);
                flake.vy = 14.0f + static_cast<float>(rng() % 28);
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

    const float logo_w = FeTitleLayout::logo_w;
    const float logo_h = logo_w / kLogoAspect;
    const float logo_x = (vw - logo_w) * 0.5f;
    const float logo_y = FeTitleLayout::logo_y;
    const float logo_cx = logo_x + logo_w * 0.5f;
    const float logo_cy = logo_y + logo_h * 0.5f;

    update_snow(dt, logo_cx, logo_cy, logo_w);

    append_fullwindow_sky(frame.sprites, viewport, 0.62f, 0.74f, 0.88f, 0.88f, 0.92f, 0.98f);

    MenuSprite mountain{};
    mountain.texture_index = kTexMenuMountain;
    mountain.x = vpx(viewport, FeTitleLayout::mountain_x);
    mountain.y = vpy(viewport, FeTitleLayout::mountain_y);
    mountain.w = vpw(viewport, FeTitleLayout::mountain_w);
    mountain.h = vph(viewport, FeTitleLayout::mountain_h);
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
    fog.texture_index = kTexMenuSnow;
    fog.x = vpx(viewport, 0.0f);
    fog.y = vpy(viewport, FeTitleLayout::fog_y);
    fog.w = vpw(viewport, vw);
    fog.h = vph(viewport, vh - FeTitleLayout::fog_y);
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
        snow.texture_index = kTexMenuSnow;
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
    logo.texture_index = kTexMenuLogo;
    logo.x = vpx(viewport, logo_x);
    logo.y = vpy(viewport, logo_y);
    logo.w = vpw(viewport, logo_w);
    logo.h = vph(viewport, logo_h);
    logo.a = 1.0f;
    append_sprite(frame.sprites, logo);

    for (const Snowflake& flake : g_title.burst) {
        MenuSprite snow{};
        snow.texture_index = kTexMenuSnow;
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

    const float blink = 0.55f + 0.45f * (0.5f + 0.5f * std::sin(time_sec * 3.0f));
    const char* press_text = fe_string_title_press();
    const float press_scale = FeTitleLayout::press_scale;
    const float press_w = measure_text_width_virtual(press_text, press_scale);
    append_text(frame.sprites,
                viewport,
                press_text,
                (vw - press_w) * 0.5f,
                FeTitleLayout::press_y,
                press_scale,
                0.88f,
                0.90f,
                0.94f,
                blink);

    const char* copy_text = fe_string_title_copyright();
    const float copy_scale = FeTitleLayout::copyright_scale;
    const float copy_w = measure_text_width_virtual(copy_text, copy_scale);
    append_text(frame.sprites,
                viewport,
                copy_text,
                (vw - copy_w) * 0.5f,
                FeTitleLayout::copyright_y,
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

    const FeMainMenuLayoutValues& layout = fe_main_menu_layout();
    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    append_menu_backdrop(frame.sprites, viewport);
    append_main_menu_left_panel(frame.sprites, viewport, layout.column_divider_x);
    append_main_menu_orange_frame(frame.sprites, viewport);

    const float orange3_w = 52.0f;
    const float orange3_h = 64.0f;
    append_texture_rect(frame.sprites,
                        viewport,
                        kTexMenuBrandLogo,
                        layout.orange_three_x,
                        layout.orange_three_y,
                        orange3_w,
                        orange3_h,
                        FeBrandAtlas::kOrangeThree);

    const float wordmark_w = layout.corner_logo_w;
    const float wordmark_h = 36.0f;
    append_texture_rect(frame.sprites,
                        viewport,
                        kTexMenuBrandLogo,
                        layout.corner_logo_x,
                        layout.corner_logo_y,
                        wordmark_w,
                        wordmark_h,
                        FeBrandAtlas::kWordmark,
                        0.9f);

    append_dashed_line_h(frame.sprites, viewport, 16.0f, vw - 16.0f, layout.header_divider_y);
    append_dashed_line_v(frame.sprites,
                         viewport,
                         layout.column_divider_x,
                         layout.header_divider_y + 4.0f,
                         vh - 56.0f);

    append_text(frame.sprites,
                viewport,
                fe_string_main_menu_title(),
                layout.header_title_x,
                layout.header_title_y,
                layout.header_title_scale,
                0.98f,
                0.98f,
                1.0f,
                1.0f);

    const int items = item_count < fe_mainmenu_item_count() ? item_count : fe_mainmenu_item_count();
    const int sel = selected_item < 0 ? 0 : (selected_item >= items ? items - 1 : selected_item);

    append_text(frame.sprites,
                viewport,
                fe_mainmenu_description(sel),
                layout.desc_x,
                layout.desc_y,
                layout.desc_scale,
                0.96f,
                0.98f,
                1.0f,
                1.0f);

    MenuSprite mountain{};
    mountain.texture_index = kTexMenuMountain;
    mountain.x = vpx(viewport, layout.mountain_x);
    mountain.y = vpy(viewport, layout.mountain_y);
    mountain.w = vpw(viewport, layout.mountain_w);
    mountain.h = vph(viewport, layout.mountain_h);
    mountain.r = 0.55f;
    mountain.g = 0.68f;
    mountain.b = 0.82f;
    mountain.a = 1.0f;
    append_sprite(frame.sprites, mountain);

    const float sel_y = layout.menu_y + static_cast<float>(sel) * layout.menu_line_h - 5.0f;
    append_menu_list_highlight(
        frame.sprites, viewport, layout.sel_bar_x, sel_y, layout.sel_bar_w, layout.sel_bar_h);

    for (int i = 0; i < items; ++i) {
        const bool is_online = (i == fe_mainmenu_item_count() - 1);
        append_menu_list_label(frame.sprites,
                               viewport,
                               fe_mainmenu_label(i),
                               layout.menu_x,
                               layout.menu_y + static_cast<float>(i) * layout.menu_line_h,
                               layout.menu_scale,
                               layout.menu_scale_sel,
                               i == sel,
                               is_online);
    }

    float snow_r = 0.95f;
    float snow_g = 0.97f;
    float snow_b = 1.0f;
    if (const HostVisualEffectsMainMenu* active = host_visualfx_mainmenu_active()) {
        snow_r = host_visualfx_mainmenu_snow_r(active);
        snow_g = host_visualfx_mainmenu_snow_g(active);
        snow_b = host_visualfx_mainmenu_snow_b(active);
    }
    static const float kFlake[][2] = {
        {468.0f, 248.0f}, {512.0f, 198.0f}, {548.0f, 286.0f}, {498.0f, 332.0f},
        {572.0f, 228.0f}, {438.0f, 302.0f}, {524.0f, 356.0f},
    };
    for (const auto& pos : kFlake) {
        if (pos[0] < layout.column_divider_x + 8.0f) {
            continue;
        }
        const float sz = 5.0f + 3.0f * std::sin(time_sec * 2.0f + pos[0] * 0.02f);
        MenuSprite flake{};
        flake.texture_index = kTexMenuSnow;
        flake.x = vpx(viewport, pos[0]);
        flake.y = vpy(viewport, pos[1]);
        flake.w = vpw(viewport, sz);
        flake.h = vph(viewport, sz);
        flake.r = snow_r;
        flake.g = snow_g;
        flake.b = snow_b;
        flake.a = 0.45f;
        append_sprite(frame.sprites, flake);
    }

    append_main_menu_controller_hints(
        frame.sprites, viewport, layout.hint_y, layout.hint_scale);
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

    const FeMountainLayoutValues& layout = fe_mountain_layout();
    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    append_retail_menu_sky(frame.sprites, viewport);

    int map_texture = kTexMapPanorama;
    if (selected_peak == 0 && vulkan_menu_texture_ready(kTexMapPeakA)) {
        map_texture = kTexMapPeakA;
    } else if (selected_peak == 1 && vulkan_menu_texture_ready(kTexMapPeakB)) {
        map_texture = kTexMapPeakB;
    } else if (selected_peak == 2 && vulkan_menu_texture_ready(kTexMapPeakC)) {
        map_texture = kTexMapPeakC;
    } else if (!vulkan_menu_texture_ready(kTexMapPanorama)) {
        map_texture = kTexMenuMountain;
    }

    const float pan = 0.5f + 0.12f * std::sin(time_sec * 0.35f);
    MenuSprite panorama{};
    panorama.texture_index = map_texture;
    panorama.x = vpx(viewport, layout.map_panorama_x);
    panorama.y = vpy(viewport, layout.map_panorama_y);
    panorama.w = vpw(viewport, layout.map_panorama_w);
    panorama.h = vph(viewport, layout.map_panorama_h);
    panorama.u0 = pan - 0.35f;
    panorama.u1 = pan + 0.35f;
    panorama.v0 = 0.0f;
    panorama.v1 = 1.0f;
    panorama.a = 1.0f;
    append_sprite(frame.sprites, panorama);

    MenuSprite vignette{};
    vignette.texture_index = kTexMenuSnow;
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

    const int peaks = peak_count < fe_mountain_peak_count() ? peak_count : fe_mountain_peak_count();
    const float card_w = layout.peak_card_w;
    const float card_y = layout.peak_y;
    const float gap = layout.peak_gap;
    const float total_w = static_cast<float>(peaks) * card_w + static_cast<float>(peaks - 1) * gap;
    float card_x = (vw - total_w) * 0.5f;

    for (int i = 0; i < peaks; ++i) {
        const bool selected = (i == selected_peak);
        const float scale = selected ? layout.peak_scale_sel : layout.peak_scale;
        const float pulse = selected ? (0.92f + 0.08f * std::sin(time_sec * 6.0f)) : 0.78f;
        append_text(frame.sprites,
                    viewport,
                    fe_mountain_peak_label(i),
                    card_x,
                    card_y,
                    scale,
                    selected ? 0.98f : 0.50f,
                    selected ? 0.98f : 0.54f,
                    selected ? 1.0f : 0.58f,
                    selected ? pulse : 0.70f);
        card_x += card_w + gap;
    }

    append_text(frame.sprites,
                viewport,
                fe_mainmenu_label(1),
                layout.header_x,
                layout.header_y,
                layout.header_scale * 0.72f,
                0.94f,
                0.96f,
                0.98f,
                0.95f);
    append_text(frame.sprites,
                viewport,
                fe_string_mountain_select_peak(),
                layout.header_x,
                layout.header_y + 28.0f,
                1.05f,
                0.55f,
                0.58f,
                0.62f,
                0.88f);
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

    append_options_backdrop(frame.sprites, viewport);

    append_text(frame.sprites,
                viewport,
                "Options",
                FeOptionsLayout::title_x,
                FeOptionsLayout::title_y,
                FeOptionsLayout::title_scale,
                0.94f,
                0.96f,
                0.98f,
                1.0f);

    const int items = item_count < fe_options_item_count() ? item_count : fe_options_item_count();

    for (int i = 0; i < items; ++i) {
        append_options_list_label(frame.sprites,
                                  viewport,
                                  fe_options_label(i),
                                  FeOptionsLayout::menu_x,
                                  FeOptionsLayout::menu_y +
                                      static_cast<float>(i) * FeOptionsLayout::menu_line_h,
                                  FeOptionsLayout::menu_scale,
                                  FeOptionsLayout::menu_scale_sel,
                                  i == selected_item);
    }

    (void)time_sec;
}

namespace {

void format_option_row(char* out,
                       size_t out_size,
                       const char* label,
                       const char* const* choices,
                       int choice_count,
                       int choice_index) {
    if (!out || out_size == 0 || !label) {
        return;
    }
    if (!choices || choice_count <= 0) {
        std::snprintf(out, out_size, "%s", label);
        return;
    }
    if (choice_index < 0) {
        choice_index = 0;
    }
    if (choice_index >= choice_count) {
        choice_index = choice_count - 1;
    }
    std::snprintf(out, out_size, "%s: %s", label, choices[choice_index]);
}

} // namespace

void build_options_game_screen(MenuFrame& frame,
                               const MenuViewport& viewport,
                               float time_sec,
                               int selected_row,
                               int row_count,
                               const int* row_values,
                               int value_columns) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f || row_count <= 0) {
        return;
    }

    append_options_sub_backdrop(frame.sprites, viewport);

    append_text(frame.sprites,
                viewport,
                "Game Options",
                FeOptionsLayout::title_x,
                40.0f,
                FeOptionsLayout::title_scale * 0.88f,
                0.94f,
                0.96f,
                0.98f,
                1.0f);

    static const char* kDifficulty[] = {"Easy", "Medium", "Hard"};
    static const char* kCamera[] = {"Dynamic", "Classic", "Near"};
    static const char* kTrick[] = {"Slow", "Normal", "Fast"};
    static const char* kCrash[] = {"Off", "On"};
    struct RowSpec {
        const char* label;
        const char* const* choices;
        int choice_count;
    };
    static const RowSpec kSpecs[] = {
        {"Difficulty", kDifficulty, 3},
        {"Camera", kCamera, 3},
        {"Trick speed", kTrick, 3},
        {"Crash resistance", kCrash, 2},
        {"Back", nullptr, 0},
    };
    const int spec_count = static_cast<int>(sizeof(kSpecs) / sizeof(kSpecs[0]));
    const int rows = row_count < spec_count ? row_count : spec_count;
    for (int i = 0; i < rows; ++i) {
        const bool selected = (i == selected_row);
        char line[96]{};
        if (kSpecs[i].choices) {
            int vi = row_values ? row_values[i] : 0;
            if (value_columns > 0 && i < value_columns) {
                vi = row_values[i];
            }
            format_option_row(line, sizeof(line), kSpecs[i].label, kSpecs[i].choices, kSpecs[i].choice_count, vi);
        } else {
            std::snprintf(line, sizeof(line), "%s", kSpecs[i].label);
        }
        append_options_list_label(frame.sprites,
                                  viewport,
                                  line,
                                  FeOptionsSubLayout::row_x,
                                  FeOptionsSubLayout::row_y +
                                      static_cast<float>(i) * FeOptionsSubLayout::row_line_h,
                                  FeOptionsSubLayout::row_scale,
                                  FeOptionsSubLayout::row_scale_sel,
                                  selected);
    }
}

void build_options_sound_screen(MenuFrame& frame,
                                const MenuViewport& viewport,
                                float time_sec,
                                int selected_row,
                                int row_count,
                                const int* row_values,
                                int value_columns) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f || row_count <= 0) {
        return;
    }

    append_options_sub_backdrop(frame.sprites, viewport);

    append_text(frame.sprites,
                viewport,
                "Sound Options",
                FeOptionsLayout::title_x,
                40.0f,
                FeOptionsLayout::title_scale * 0.88f,
                0.94f,
                0.96f,
                0.98f,
                1.0f);

    static const char* kOnOff[] = {"Off", "On"};
    const int rows = row_count < 5 ? row_count : 5;

    for (int i = 0; i < rows; ++i) {
        const bool selected = (i == selected_row);
        char line[96]{};
        int vi = 0;
        if (row_values && i < value_columns) {
            vi = row_values[i];
        }
        if (i == 0) {
            std::snprintf(line, sizeof(line), "Master volume: %d", vi);
        } else if (i == 4) {
            std::snprintf(line, sizeof(line), "Back");
        } else {
            const char* labels[] = {"", "Music", "SFX", "Commentary"};
            format_option_row(line, sizeof(line), labels[i], kOnOff, 2, vi);
        }
        append_options_list_label(frame.sprites,
                                  viewport,
                                  line,
                                  FeOptionsSubLayout::row_x,
                                  FeOptionsSubLayout::row_y +
                                      static_cast<float>(i) * FeOptionsSubLayout::row_line_h,
                                  FeOptionsSubLayout::row_scale,
                                  FeOptionsSubLayout::row_scale_sel,
                                  selected);
    }

    (void)time_sec;
}

void build_fe_info_screen(MenuFrame& frame,
                          const MenuViewport& viewport,
                          float time_sec,
                          const char* title,
                          const char* body) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f) {
        return;
    }

    append_options_backdrop(frame.sprites, viewport);

    append_text(frame.sprites,
                viewport,
                title ? title : "",
                FeOptionsLayout::title_x,
                FeOptionsLayout::title_y,
                FeOptionsLayout::title_scale * 0.82f,
                0.94f,
                0.96f,
                0.98f,
                1.0f);

    if (body && body[0]) {
        append_text(frame.sprites,
                    viewport,
                    body,
                    FeOptionsLayout::menu_x,
                    FeOptionsLayout::menu_y + 28.0f,
                    FeOptionsLayout::menu_scale * 0.70f,
                    0.72f,
                    0.76f,
                    0.80f,
                    0.82f);
    }
}

void build_fe_list_screen(MenuFrame& frame,
                          const MenuViewport& viewport,
                          float time_sec,
                          const char* header_title,
                          const char* const* labels,
                          const char* const* descriptions,
                          int item_count,
                          int selected_item) {
    frame.time_sec = time_sec;
    frame.width = viewport.window_width;
    frame.height = viewport.window_height;
    frame.viewport = viewport;
    frame.sprites.clear();

    if (viewport.width < 8.0f || viewport.height < 8.0f || item_count <= 0 || !labels) {
        return;
    }

    const FeMainMenuLayoutValues& layout = fe_main_menu_layout();
    constexpr float vw = static_cast<float>(kTitleVirtualWidth);
    constexpr float vh = static_cast<float>(kTitleVirtualHeight);

    append_menu_backdrop(frame.sprites, viewport);

    append_texture_rect(frame.sprites,
                        viewport,
                        kTexMenuOverlay,
                        0.0f,
                        vh - 72.0f,
                        vw,
                        72.0f,
                        FeOverlayAtlas::kTreeLine,
                        0.35f);

    MenuSprite mountain{};
    mountain.texture_index = kTexMenuMountain;
    mountain.x = vpx(viewport, layout.mountain_x);
    mountain.y = vpy(viewport, layout.mountain_y);
    mountain.w = vpw(viewport, layout.mountain_w);
    mountain.h = vph(viewport, layout.mountain_h);
    mountain.r = 0.55f;
    mountain.g = 0.68f;
    mountain.b = 0.82f;
    mountain.a = 1.0f;
    append_sprite(frame.sprites, mountain);

    append_texture_rect(frame.sprites,
                        viewport,
                        kTexMenuBrandLogo,
                        layout.orange_three_x,
                        layout.orange_three_y,
                        52.0f,
                        64.0f,
                        FeBrandAtlas::kOrangeThree);

    append_texture_rect(frame.sprites,
                        viewport,
                        kTexMenuBrandLogo,
                        layout.corner_logo_x,
                        layout.corner_logo_y,
                        layout.corner_logo_w,
                        36.0f,
                        FeBrandAtlas::kWordmark,
                        0.9f);

    append_dashed_line_h(frame.sprites, viewport, 16.0f, vw - 16.0f, layout.header_divider_y);
    append_dashed_line_v(frame.sprites,
                         viewport,
                         layout.column_divider_x,
                         layout.header_divider_y + 4.0f,
                         vh - 56.0f);

    append_text(frame.sprites,
                viewport,
                header_title ? header_title : "",
                layout.header_title_x,
                layout.header_title_y,
                layout.header_title_scale,
                0.96f,
                0.98f,
                1.0f,
                1.0f);

    const int items = item_count;
    const int sel = selected_item < 0 ? 0 : (selected_item >= items ? items - 1 : selected_item);

    if (descriptions && descriptions[sel] && descriptions[sel][0]) {
        append_text(frame.sprites,
                    viewport,
                    descriptions[sel],
                    layout.desc_x,
                    layout.desc_y,
                    layout.desc_scale,
                    0.92f,
                    0.94f,
                    0.98f,
                    0.95f);
    }

    const float sel_y = layout.menu_y + static_cast<float>(sel) * layout.menu_line_h - 4.0f;
    append_menu_list_highlight(
        frame.sprites, viewport, layout.sel_bar_x, sel_y, layout.sel_bar_w, layout.sel_bar_h);

    for (int i = 0; i < items; ++i) {
        append_menu_list_label(frame.sprites,
                               viewport,
                               labels[i] ? labels[i] : "",
                               layout.menu_x,
                               layout.menu_y + static_cast<float>(i) * layout.menu_line_h,
                               layout.menu_scale,
                               layout.menu_scale_sel,
                               i == sel,
                               false);
    }

    append_text(frame.sprites,
                viewport,
                fe_string_main_menu_hints(),
                layout.hint_x,
                layout.hint_y,
                layout.hint_scale,
                0.48f,
                0.50f,
                0.54f,
                0.82f);
}

} // namespace host
