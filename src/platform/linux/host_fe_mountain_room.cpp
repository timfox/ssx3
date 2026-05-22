#include "platform/host_fe_mountain_room.h"

#include "platform/host_fe_mountainroom_state.h"
#include "platform/host_fe_strings.h"
#include "platform/host_visualfx_menu.h"

#include <cstring>

namespace host {

HostFEStateMountainRoom* fe_mountainroom_as_state(void* self) {
    auto* room = static_cast<HostFEStateMountainRoom*>(self);
    if (!room || room->magic != kFEMountainRoomMagic) {
        return nullptr;
    }
    return room;
}

int fe_mountain_room_peak_count() { return fe_mountain_peak_count(); }

int fe_mountain_room_get_peak(void* self) {
    auto* room = fe_mountainroom_as_state(self);
    return room ? room->selected_peak : 0;
}

void fe_mountain_room_set_peak(void* self, int index) {
    auto* room = fe_mountainroom_as_state(self);
    if (!room) {
        return;
    }
    if (index < 0) {
        index = 0;
    }
    if (index >= fe_mountain_peak_count()) {
        index = fe_mountain_peak_count() - 1;
    }
    room->selected_peak = index;
}

void fe_mountain_room_navigate_peak(void* self, int delta) {
    fe_mountain_room_set_peak(self, fe_mountain_room_get_peak(self) + delta);
}

} // namespace host

extern "C" {

void* host_fe_mountain_room_create(const char* asset_dir) {
    auto* room = new host::HostFEStateMountainRoom();
    if (asset_dir) {
        std::strncpy(room->asset_dir, asset_dir, sizeof(room->asset_dir) - 1);
    }
    room->visual_fx = host::host_visualfx_mainmenu_create();
    return room;
}

void host_fe_mountain_room_destroy(void* self) {
    auto* room = host::fe_mountainroom_as_state(self);
    if (!room) {
        return;
    }
    if (room->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(room->visual_fx);
        room->visual_fx = nullptr;
    }
    delete room;
}

} // extern "C"
