#include "platform/host_graphics.h"

#include "platform/host_gs.h"
#include "platform/host_log.h"

#include <cstdio>
#include "platform/host_menu_frame.h"
#include "platform/host_visualfx_menu.h"
#include "visualfx/crowdrender2d.h"

#include <atomic>

namespace {
std::atomic<int> g_tex_id{1};
} // namespace

extern "C" {

int cPSPGraphicsMan_NewBindTexID(void* gfx, void* tex, int a2) {
    (void)gfx;
    (void)tex;
    (void)a2;
    const int id = g_tex_id.fetch_add(1);
    if (host::gs_renderer_active()) {
        char log_buf[72];
        std::snprintf(log_buf, sizeof(log_buf), "cPSPGraphicsMan_NewBindTexID id=%d (Vulkan GS)", id);
        host::host_log("gfx", log_buf);
    } else {
        host::host_stub_log("cPSPGraphicsMan_NewBindTexID");
    }
    return id;
}

int cPSPGraphicsMan_NewNonBindTexID(void* gfx, void* tex, int a2) {
    (void)gfx;
    (void)tex;
    (void)a2;
    const int id = g_tex_id.fetch_add(1);
    if (host::gs_renderer_active()) {
        char log_buf[80];
        std::snprintf(log_buf, sizeof(log_buf), "cPSPGraphicsMan_NewNonBindTexID id=%d (Vulkan GS)", id);
        host::host_log("gfx", log_buf);
    } else {
        host::host_stub_log("cPSPGraphicsMan_NewNonBindTexID");
    }
    return id;
}

void cRenderStateMan_SnowFlakeColourR(void* state, float value) {
    (void)state;
    if (auto* fx = host::host_visualfx_mainmenu_active()) {
        host::host_visualfx_mainmenu_set_snow_colour(
            fx, value, host::host_visualfx_mainmenu_snow_g(fx), host::host_visualfx_mainmenu_snow_b(fx));
    }
}

void cRenderStateMan_SnowFlakeColourG(void* state, float value) {
    (void)state;
    if (auto* fx = host::host_visualfx_mainmenu_active()) {
        host::host_visualfx_mainmenu_set_snow_colour(
            fx, host::host_visualfx_mainmenu_snow_r(fx), value, host::host_visualfx_mainmenu_snow_b(fx));
    }
}

void cRenderStateMan_SnowFlakeColourB(void* state, float value) {
    (void)state;
    if (auto* fx = host::host_visualfx_mainmenu_active()) {
        host::host_visualfx_mainmenu_set_snow_colour(
            fx, host::host_visualfx_mainmenu_snow_r(fx), host::host_visualfx_mainmenu_snow_g(fx), value);
    }
}

void cGraphicsMan_AddBlendedMatrix(void* gfx, void* matrix) {
    (void)gfx;
    (void)matrix;
    host::host_stub_log("cGraphicsMan_AddBlendedMatrix");
}

void apply_retail_menu_snow_colours(void* visual_fx) {
    cRenderStateMan_SnowFlakeColourR(nullptr, 0.92f);
    cRenderStateMan_SnowFlakeColourG(nullptr, 0.95f);
    cRenderStateMan_SnowFlakeColourB(nullptr, 1.0f);
    if (visual_fx) {
        host::host_visualfx_mainmenu_set_snow_colour(
            static_cast<host::HostVisualEffectsMainMenu*>(visual_fx), 0.92f, 0.95f, 1.0f);
    }
}

void cVisualEffectsMainMenu_cVisualEffectsMainMenu(void* fx, void* a1) {
    (void)a1;
    auto* menu = static_cast<host::HostVisualEffectsMainMenu*>(fx);
    if (!menu) {
        return;
    }
    static bool crowd_ready = false;
    if (!crowd_ready) {
        cCrowdRender2D_init();
        crowd_ready = true;
    }
    host::host_visualfx_mainmenu_reset(menu);
    host::host_visualfx_mainmenu_set_active(menu);
    apply_retail_menu_snow_colours(menu);
    host::host_log("gfx", "cVisualEffectsMainMenu_cVisualEffectsMainMenu (retail 0x24D2F0 → Vulkan)");
}

void cVisualEffectsMainMenu_render(void* fx, void* menu_frame) {
    (void)fx;
    if (!menu_frame) {
        return;
    }
    host::gs_submit_menu_frame(*static_cast<const host::MenuFrame*>(menu_frame));
}

} // extern "C"
