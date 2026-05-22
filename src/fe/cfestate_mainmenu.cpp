// Retail cFEStateMainMenu @ 0x194AA8 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_mainmenu_state.h"
#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"
#include "platform/host_fe_strings.h"

extern "C" {
#endif

void cFEStateMainMenu_onCreateScreen(void* self) {
    auto* mm = host::fe_mainmenu_as_state(self);
    if (!mm) {
        return;
    }
    host::host_log("fe", "cFEStateMainMenu_onCreateScreen (retail 0x194AA8)");

    if (!mm->visual_fx) {
        mm->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(mm->visual_fx, nullptr);

    mm->selected = 0;
    mm->initialized = 1;
}

void cFEStateMainMenu_onDestroyScreen(void* self) {
    auto* mm = host::fe_mainmenu_as_state(self);
    if (!mm) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    mm->initialized = 0;
    host::host_log("fe", "cFEStateMainMenu_onDestroyScreen");
}

void cFEStateMainMenu_onUpdate(void* self) {
    auto* mm = host::fe_mainmenu_as_state(self);
    if (!mm || !mm->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    mm->frame.time_sec += host::fe_frame_dt();
    host::build_main_menu_screen(
        mm->frame, viewport, mm->frame.time_sec, mm->selected, host::fe_mainmenu_item_count());
    cVisualEffectsMainMenu_render(mm->visual_fx, &mm->frame);
}

void cFEStateMainMenu_onWidgetCreate(void* self, void* widget) {
    (void)self;
    (void)widget;
    host::host_log("fe", "cFEStateMainMenu_onWidgetCreate (retail 0x194D18, stub)");
}

void cFEStateMainMenu_onWidgetEvent(void* self, void* widget, int event) {
    (void)widget;
    auto* mm = host::fe_mainmenu_as_state(self);
    if (!mm) {
        return;
    }
    if (event == 2 || event == 4) {
        host::fe_mainmenu_navigate(self, -1);
    } else if (event == 3 || event == 5) {
        host::fe_mainmenu_navigate(self, 1);
    } else if (event >= 0 && event < host::fe_mainmenu_item_count()) {
        host::fe_mainmenu_set_selected(self, event);
    }
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
