// Retail cFEStateTitle screen hooks @ 0x194778 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_disc.h"
#include "platform/host_fe_context.h"
#include "platform/host_fe_title_state.h"
#include "platform/host_menu_layout.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_visualfx_menu.h"

extern "C" {
#endif

void cFEStateTitle_onCreateScreen(void* self) {
    auto* fe = host::fe_title_as_state(self);
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
    apply_retail_menu_snow_colours(fe->visual_fx);
    fe->menu_time = 0.0f;
    fe->initialized = 1;
}

void cFEStateTitle_onDestroyScreen(void* self) {
    auto* fe = host::fe_title_as_state(self);
    if (!fe) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    host::reset_title_screen_state();
    fe->initialized = 0;
    host::host_log("fe", "cFEStateTitle_onDestroyScreen");
}

void cFEStateTitle_onUpdate(void* self) {
    auto* fe = host::fe_title_as_state(self);
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

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
