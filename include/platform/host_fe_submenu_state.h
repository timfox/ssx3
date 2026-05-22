#ifndef PLATFORM_HOST_FE_SUBMENU_STATE_H
#define PLATFORM_HOST_FE_SUBMENU_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEPreviewsMagic = 0x46455056u; /* "FEPV" */
constexpr std::uint32_t kFEMultiplayMagic = 0x46454D50u; /* "FEMP" */
constexpr std::uint32_t kFESingleEventMagic = 0x46455356u; /* "FESV" */
constexpr std::uint32_t kFEPeakRoomMagic = 0x4645504Bu; /* "FEPK" */

struct HostFESubmenuScreen {
    std::uint32_t magic = 0;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int initialized = 0;
    int item_count = 0;
    int peak_index = 0;
    char title_buf[128]{};
};

HostFESubmenuScreen* fe_submenu_as_state(void* self, std::uint32_t magic);
HostFESubmenuScreen* fe_submenu_create(std::uint32_t magic, const char* asset_dir, int peak_index);
void fe_submenu_destroy(void* self, std::uint32_t magic);
void fe_submenu_on_create_screen(HostFESubmenuScreen* screen,
                                 const char* log_label,
                                 const char* retail_addr);
void fe_submenu_on_destroy_screen(HostFESubmenuScreen* screen);
void fe_submenu_on_update_list(HostFESubmenuScreen* screen);
void fe_submenu_navigate(HostFESubmenuScreen* screen, int delta);

} // namespace host

#endif /* PLATFORM_HOST_FE_SUBMENU_STATE_H */
