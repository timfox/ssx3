// Retail cFEStateOptions @ 0x188800 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_options_state.h"
#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"
#include "platform/host_fe_strings.h"

extern "C" {
#endif

void cFEStateOptions_onCreateScreen(void* self) {
    auto* opt = host::fe_options_as_state(self);
    if (!opt) {
        return;
    }
    host::host_log("fe", "cFEStateOptions_onCreateScreen (retail 0x188800)");

    if (!opt->visual_fx) {
        opt->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(opt->visual_fx, nullptr);
    opt->selected = 0;
    opt->initialized = 1;
}

void cFEStateOptions_onDestroyScreen(void* self) {
    auto* opt = host::fe_options_as_state(self);
    if (!opt) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    opt->initialized = 0;
    host::host_log("fe", "cFEStateOptions_onDestroyScreen");
}

void cFEStateOptions_onUpdate(void* self) {
    auto* opt = host::fe_options_as_state(self);
    if (!opt || !opt->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    opt->frame.time_sec += host::fe_frame_dt();
    host::build_options_screen(
        opt->frame, viewport, opt->frame.time_sec, opt->selected, host::fe_options_item_count());
    cVisualEffectsMainMenu_render(opt->visual_fx, &opt->frame);
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
