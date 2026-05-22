#include "platform/host_fe_mainmenu.h"

#include "platform/host_fe_mainmenu_state.h"
#include "platform/host_visualfx_menu.h"

#include <cstring>

namespace host {

HostFEStateMainMenu* fe_mainmenu_as_state(void* self) {
    auto* mm = static_cast<HostFEStateMainMenu*>(self);
    if (!mm || mm->magic != kFEMainMenuMagic) {
        return nullptr;
    }
    return mm;
}

constexpr int kMenuItemCount = 5;

int fe_mainmenu_get_selected(void* self) {
    auto* mm = fe_mainmenu_as_state(self);
    return mm ? mm->selected : 0;
}

void fe_mainmenu_set_selected(void* self, int index) {
    auto* mm = fe_mainmenu_as_state(self);
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
    auto* mm = host::fe_mainmenu_as_state(self);
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

} // extern "C"
