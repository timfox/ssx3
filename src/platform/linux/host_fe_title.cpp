#include "platform/host_fe_title.h"

#include "platform/host_disc.h"
#include "platform/host_fe_context.h"
#include "platform/host_fe.h"
#include "platform/host_fe_session.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>
#include <cstring>

namespace host {

constexpr std::uint32_t kFEStateTitleMagic = 0x46455449u; /* "FETI" */

struct HostFEStateTitle {
    std::uint32_t magic = kFEStateTitleMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    float menu_time = 0.0f;
    int initialized = 0;
};

HostFEStateTitle* as_fe(void* self) {
    auto* fe = static_cast<HostFEStateTitle*>(self);
    if (!fe || fe->magic != kFEStateTitleMagic) {
        return nullptr;
    }
    return fe;
}

void apply_retail_snow_colours(HostVisualEffectsMainMenu* fx) {
    if (!fx) {
        return;
    }
    cRenderStateMan_SnowFlakeColourR(nullptr, 0.92f);
    cRenderStateMan_SnowFlakeColourG(nullptr, 0.95f);
    cRenderStateMan_SnowFlakeColourB(nullptr, 1.0f);
}

void* host_fe_title_create(const char* asset_dir) {
    auto* fe = new HostFEStateTitle();
    if (asset_dir) {
        std::strncpy(fe->asset_dir, asset_dir, sizeof(fe->asset_dir) - 1);
    }
    fe->visual_fx = host_visualfx_mainmenu_create();
    return fe;
}

void host_fe_title_destroy(void* self) {
    auto* fe = as_fe(self);
    if (!fe) {
        return;
    }
    if (fe->visual_fx) {
        host_visualfx_mainmenu_set_active(nullptr);
        host_visualfx_mainmenu_destroy(fe->visual_fx);
        fe->visual_fx = nullptr;
    }
    delete fe;
}

bool run_fe_title_menu_loop(Renderer* renderer, const std::string& asset_dir) {
    return run_fe_menu_session(renderer, asset_dir);
}

} // namespace host

extern "C" {

void cFEStateTitle_onCreateScreen(void* self) {
    auto* fe = host::as_fe(self);
    if (!fe) {
        host::host_log("fe", "cFEStateTitle_onCreateScreen: null state");
        return;
    }

    host::host_log("fe", "cFEStateTitle_onCreateScreen (retail 0x194778)");

    if (host::disc().exists("data/ui/fe_1.ssh")) {
        host::host_log("fe", "disc UI: data/ui/fe_1.ssh");
    } else {
        host::host_log("fe", "missing data/ui/fe_1.ssh");
    }

    if (!fe->visual_fx) {
        fe->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(fe->visual_fx, nullptr);
    host::apply_retail_snow_colours(fe->visual_fx);
    fe->menu_time = 0.0f;
    fe->initialized = 1;
}

void cFEStateTitle_onDestroyScreen(void* self) {
    auto* fe = host::as_fe(self);
    if (!fe) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    host::reset_title_screen_state();
    fe->initialized = 0;
    host::host_log("fe", "cFEStateTitle_onDestroyScreen");
}

void cFEStateTitle_onUpdate(void* self) {
    auto* fe = host::as_fe(self);
    if (!fe || !fe->initialized || !fe->visual_fx) {
        return;
    }

    fe->menu_time += host::fe_frame_dt();
    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    host::host_visualfx_mainmenu_build_frame(
        fe->visual_fx, fe->frame, viewport, fe->menu_time, host::fe_frame_dt());
    cVisualEffectsMainMenu_render(fe->visual_fx, &fe->frame);
}

} // extern "C"
