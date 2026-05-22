#ifndef PLATFORM_HOST_FE_MAINMENU_STATE_H
#define PLATFORM_HOST_FE_MAINMENU_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEMainMenuMagic = 0x46454D4Du; /* "FEMM" */

struct HostFEStateMainMenu {
    std::uint32_t magic = kFEMainMenuMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int initialized = 0;
};

HostFEStateMainMenu* fe_mainmenu_as_state(void* self);
int fe_mainmenu_get_selected(void* self);
void fe_mainmenu_set_selected(void* self, int index);
void fe_mainmenu_navigate(void* self, int delta);

} // namespace host

#endif /* PLATFORM_HOST_FE_MAINMENU_STATE_H */
