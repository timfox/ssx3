// Retail cFEStateOptionsGame @ 0x188F28 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_options_game_state.h"
#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

extern "C" {
#endif

void cFEStateOptionsGame_onCreateScreen(void* self) {
    auto* og = host::fe_options_game_as_state(self);
    if (!og) {
        return;
    }
    host::host_log("fe", "cFEStateOptionsGame_onCreateScreen (retail 0x188F28)");

    if (!og->visual_fx) {
        og->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(og->visual_fx, nullptr);
    og->selected = 0;
    og->initialized = 1;
}

void cFEStateOptionsGame_onDestroyScreen(void* self) {
    auto* og = host::fe_options_game_as_state(self);
    if (!og) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    og->initialized = 0;
    host::host_log("fe", "cFEStateOptionsGame_onDestroyScreen");
}

void cFEStateOptionsGame_onUpdate(void* self) {
    auto* og = host::fe_options_game_as_state(self);
    if (!og || !og->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    og->frame.time_sec += host::fe_frame_dt();
    const int rows = host::fe_options_game_row_count();
    host::build_options_game_screen(
        og->frame, viewport, og->frame.time_sec, og->selected, rows, og->values, rows);
    cVisualEffectsMainMenu_render(og->visual_fx, &og->frame);
}

void cFEStateOptionsGame_onWidgetEvent(void* self, void* widget, int event) {
    (void)widget;
    auto* og = host::fe_options_game_as_state(self);
    if (!og) {
        return;
    }
    if (event == 2 || event == 4) {
        host::fe_options_game_navigate(self, -1);
    } else if (event == 3 || event == 5) {
        host::fe_options_game_navigate(self, 1);
    } else if (event == 6 || event == 7) {
        host::fe_options_game_adjust_value(self, (event == 6) ? -1 : 1);
    } else if (event >= 0 && event < host::fe_options_game_row_count()) {
        host::fe_options_game_set_selected(self, event);
    }
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
