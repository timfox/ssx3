#include "platform/host_graphics.h"

#include "platform/host_gs.h"
#include "platform/host_log.h"
#include "platform/host_menu_frame.h"
#include "platform/host_visualfx_menu.h"

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
    host::host_stub_log("cPSPGraphicsMan_NewBindTexID");
    return id;
}

int cPSPGraphicsMan_NewNonBindTexID(void* gfx, void* tex, int a2) {
    (void)gfx;
    (void)tex;
    (void)a2;
    const int id = g_tex_id.fetch_add(1);
    host::host_stub_log("cPSPGraphicsMan_NewNonBindTexID");
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

void cVisualEffectsMainMenu_cVisualEffectsMainMenu(void* fx, void* a1) {
    (void)a1;
    auto* menu = static_cast<host::HostVisualEffectsMainMenu*>(fx);
    if (!menu) {
        return;
    }
    host::host_visualfx_mainmenu_reset(menu);
    host::host_visualfx_mainmenu_set_active(menu);
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
