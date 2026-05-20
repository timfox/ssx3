#include "platform/host_visualfx_menu.h"

#include "platform/host_menu_layout.h"

#include <cstdint>

namespace host {

namespace {

constexpr std::uint32_t kFxMagic = 0x56584658u; /* "VXFX" */

struct HostVisualEffectsMainMenuImpl {
    std::uint32_t magic = kFxMagic;
    MenuRetailStyle style{};
    float time_sec = 0.0f;
};

HostVisualEffectsMainMenuImpl* as_impl(HostVisualEffectsMainMenu* ptr) {
    auto* fx = reinterpret_cast<HostVisualEffectsMainMenuImpl*>(ptr);
    if (!fx || fx->magic != kFxMagic) {
        return nullptr;
    }
    return fx;
}

HostVisualEffectsMainMenuImpl* g_active_fx = nullptr;

} // namespace

HostVisualEffectsMainMenu* host_visualfx_mainmenu_create() {
    return reinterpret_cast<HostVisualEffectsMainMenu*>(new HostVisualEffectsMainMenuImpl());
}

void host_visualfx_mainmenu_destroy(HostVisualEffectsMainMenu* fx) {
    auto* impl = as_impl(fx);
    if (g_active_fx == impl) {
        g_active_fx = nullptr;
    }
    delete impl;
}

void host_visualfx_mainmenu_reset(HostVisualEffectsMainMenu* fx) {
    auto* impl = as_impl(fx);
    if (!impl) {
        return;
    }
    impl->time_sec = 0.0f;
    reset_title_screen_state();
}

void host_visualfx_mainmenu_set_snow_colour(HostVisualEffectsMainMenu* fx, float r, float g, float b) {
    auto* impl = as_impl(fx);
    if (!impl) {
        return;
    }
    impl->style.snow_r = r;
    impl->style.snow_g = g;
    impl->style.snow_b = b;
}

float host_visualfx_mainmenu_snow_r(const HostVisualEffectsMainMenu* fx) {
    const auto* impl = as_impl(const_cast<HostVisualEffectsMainMenu*>(fx));
    return impl ? impl->style.snow_r : 1.0f;
}

float host_visualfx_mainmenu_snow_g(const HostVisualEffectsMainMenu* fx) {
    const auto* impl = as_impl(const_cast<HostVisualEffectsMainMenu*>(fx));
    return impl ? impl->style.snow_g : 1.0f;
}

float host_visualfx_mainmenu_snow_b(const HostVisualEffectsMainMenu* fx) {
    const auto* impl = as_impl(const_cast<HostVisualEffectsMainMenu*>(fx));
    return impl ? impl->style.snow_b : 1.0f;
}

void host_visualfx_mainmenu_build_frame(HostVisualEffectsMainMenu* fx,
                                        MenuFrame& frame,
                                        const MenuViewport& viewport,
                                        float time_sec,
                                        float dt) {
    auto* impl = as_impl(fx);
    if (!impl) {
        return;
    }
    impl->time_sec = time_sec;
    build_title_screen(frame, viewport, time_sec, dt, impl->style);
}

void host_visualfx_mainmenu_set_active(HostVisualEffectsMainMenu* fx) {
    g_active_fx = as_impl(fx);
}

HostVisualEffectsMainMenu* host_visualfx_mainmenu_active() {
    return reinterpret_cast<HostVisualEffectsMainMenu*>(g_active_fx);
}

} // namespace host
