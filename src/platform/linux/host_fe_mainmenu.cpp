#include "platform/host_fe_mainmenu.h"

#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>
#include <cstring>

namespace host {

constexpr std::uint32_t kFEMainMenuMagic = 0x46454D4Du; /* "FEMM" */
constexpr int kMenuItemCount = 5;

struct HostFEStateMainMenu {
    std::uint32_t magic = kFEMainMenuMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int initialized = 0;
};

HostFEStateMainMenu* as_mm(void* self) {
    auto* mm = static_cast<HostFEStateMainMenu*>(self);
    if (!mm || mm->magic != kFEMainMenuMagic) {
        return nullptr;
    }
    return mm;
}

int fe_mainmenu_item_count() { return kMenuItemCount; }

int fe_mainmenu_get_selected(void* self) {
    auto* mm = as_mm(self);
    return mm ? mm->selected : 0;
}

void fe_mainmenu_set_selected(void* self, int index) {
    auto* mm = as_mm(self);
    if (!mm) {
        return;
    }
    if (index < 0) {
        index = 0;
    }
    if (index >= kMenuItemCount) {
        index = kMenuItemCount - 1;
    }
    mm->selected = index;
}

void fe_mainmenu_navigate(void* self, int delta) {
    fe_mainmenu_set_selected(self, fe_mainmenu_get_selected(self) + delta);
}

} // namespace host

extern "C" {

void* host_fe_mainmenu_create(const char* asset_dir) {
    auto* mm = new host::HostFEStateMainMenu();
    if (asset_dir) {
        std::strncpy(mm->asset_dir, asset_dir, sizeof(mm->asset_dir) - 1);
    }
    mm->visual_fx = host::host_visualfx_mainmenu_create();
    return mm;
}

void host_fe_mainmenu_destroy(void* self) {
    auto* mm = host::as_mm(self);
    if (!mm) {
        return;
    }
    if (mm->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(mm->visual_fx);
        mm->visual_fx = nullptr;
    }
    delete mm;
}

void cFEStateMainMenu_onCreateScreen(void* self) {
    auto* mm = host::as_mm(self);
    if (!mm) {
        return;
    }
    host::host_log("fe", "cFEStateMainMenu_onCreateScreen (retail 0x194AA8)");

    if (!mm->visual_fx) {
        mm->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(mm->visual_fx, nullptr);
    cRenderStateMan_SnowFlakeColourR(nullptr, 0.98f);
    cRenderStateMan_SnowFlakeColourG(nullptr, 0.99f);
    cRenderStateMan_SnowFlakeColourB(nullptr, 1.0f);

    mm->selected = 0;
    mm->initialized = 1;
}

void cFEStateMainMenu_onDestroyScreen(void* self) {
    auto* mm = host::as_mm(self);
    if (!mm) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    mm->initialized = 0;
    host::host_log("fe", "cFEStateMainMenu_onDestroyScreen");
}

void cFEStateMainMenu_onUpdate(void* self) {
    auto* mm = host::as_mm(self);
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
    (void)self;
    (void)widget;
    (void)event;
    host::host_log("fe", "cFEStateMainMenu_onWidgetEvent (retail 0x194EA8, stub)");
}

} // extern "C"
