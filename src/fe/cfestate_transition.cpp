// Retail cFEStateTransition @ 0x194588 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_transition_state.h"
#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

extern "C" {
#endif

void cFEStateTransition_onCreateScreen(void* self) {
    auto* tr = host::fe_transition_as_state(self);
    if (!tr) {
        return;
    }
    host::host_log("fe", "cFEStateTransition_onCreateScreen (retail 0x194588)");
    if (!tr->visual_fx) {
        tr->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(tr->visual_fx, nullptr);
    tr->time_sec = 0.0f;
    tr->initialized = 1;
}

void cFEStateTransition_onDestroyScreen(void* self) {
    auto* tr = host::fe_transition_as_state(self);
    if (!tr) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    tr->initialized = 0;
}

void cFEStateTransition_onUpdate(void* self) {
    auto* tr = host::fe_transition_as_state(self);
    if (!tr || !tr->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    tr->time_sec += host::fe_frame_dt();
    tr->frame.time_sec = tr->time_sec;
    host::build_fe_info_screen(tr->frame, viewport, tr->time_sec, " ", "Loading…");
}

void cFEStateTransition_onScreenEvent(void* self, int event) {
    (void)event;
    host::host_log("fe", "cFEStateTransition_onScreenEvent (retail 0x194600, stub)");
    cFEStateTransition_onUpdate(self);
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
