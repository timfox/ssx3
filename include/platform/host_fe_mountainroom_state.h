#ifndef PLATFORM_HOST_FE_MOUNTAINROOM_STATE_H
#define PLATFORM_HOST_FE_MOUNTAINROOM_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEMountainRoomMagic = 0x46454D52u; /* "FEMR" */

struct HostFEStateMountainRoom {
    std::uint32_t magic = kFEMountainRoomMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected_peak = 0;
    int initialized = 0;
};

HostFEStateMountainRoom* fe_mountainroom_as_state(void* self);
int fe_mountain_room_peak_count();
int fe_mountain_room_get_peak(void* self);
void fe_mountain_room_set_peak(void* self, int index);
void fe_mountain_room_navigate_peak(void* self, int delta);

} // namespace host

#endif /* PLATFORM_HOST_FE_MOUNTAINROOM_STATE_H */
