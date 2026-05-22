// Retail cFEStateMountainRoom @ 0x1D2990 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_mountainroom_state.h"
#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"
#include "platform/host_fe_strings.h"

extern "C" {
#endif

void cFEStateMountainRoom_onCreateScreen(void* self) {
    auto* room = host::fe_mountainroom_as_state(self);
    if (!room) {
        return;
    }
    host::host_log("fe", "cFEStateMountainRoom_onCreateScreen (retail 0x1D2990)");

    if (!room->visual_fx) {
        room->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(room->visual_fx, nullptr);
    room->selected_peak = 0;
    room->initialized = 1;
}

void cFEStateMountainRoom_onDestroyScreen(void* self) {
    auto* room = host::fe_mountainroom_as_state(self);
    if (!room) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    room->initialized = 0;
    host::host_log("fe", "cFEStateMountainRoom_onDestroyScreen");
}

void cFEStateMountainRoom_onUpdate(void* self) {
    auto* room = host::fe_mountainroom_as_state(self);
    if (!room || !room->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    room->frame.time_sec += host::fe_frame_dt();
    host::build_mountain_room_screen(
        room->frame, viewport, room->frame.time_sec, room->selected_peak,
        host::fe_mountain_peak_count());
    cVisualEffectsMainMenu_render(room->visual_fx, &room->frame);
}

void cFEStateMountainRoom_onWidgetCreate(void* self, void* widget) {
    (void)self;
    (void)widget;
    host::host_log("fe", "cFEStateMountainRoom_onWidgetCreate (retail 0x1D2AD0, stub)");
}

void cFEStateMountainRoom_onWidgetEvent(void* self, void* widget, int event) {
    (void)widget;
    auto* room = host::fe_mountainroom_as_state(self);
    if (!room) {
        return;
    }
    if (event == 2 || event == 4) {
        host::fe_mountain_room_navigate_peak(self, -1);
    } else if (event == 3 || event == 5) {
        host::fe_mountain_room_navigate_peak(self, 1);
    } else if (event >= 0 && event < host::fe_mountain_peak_count()) {
        host::fe_mountain_room_set_peak(self, event);
    }
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
