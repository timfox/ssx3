// Retail cFEStateOptionsSound @ 0x18A298 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_options_sound_state.h"
#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

extern "C" {
#endif

void cFEStateOptionsSound_onCreateScreen(void* self) {
    auto* os = host::fe_options_sound_as_state(self);
    if (!os) {
        return;
    }
    host::host_log("fe", "cFEStateOptionsSound_onCreateScreen (retail 0x18A298)");

    if (!os->visual_fx) {
        os->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(os->visual_fx, nullptr);
    os->selected = 0;
    os->initialized = 1;
}

void cFEStateOptionsSound_onDestroyScreen(void* self) {
    auto* os = host::fe_options_sound_as_state(self);
    if (!os) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    os->initialized = 0;
    host::host_log("fe", "cFEStateOptionsSound_onDestroyScreen");
}

void cFEStateOptionsSound_onUpdate(void* self) {
    auto* os = host::fe_options_sound_as_state(self);
    if (!os || !os->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    os->frame.time_sec += host::fe_frame_dt();
    const int rows = host::fe_options_sound_row_count();
    host::build_options_sound_screen(
        os->frame, viewport, os->frame.time_sec, os->selected, rows, os->values, rows);
    cVisualEffectsMainMenu_render(os->visual_fx, &os->frame);
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
