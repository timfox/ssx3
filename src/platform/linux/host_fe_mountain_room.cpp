#include "platform/host_fe_mountain_room.h"

#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>
#include <cstring>

namespace host {

constexpr std::uint32_t kMountainMagic = 0x46454D52u; /* "FEMR" */
constexpr int kPeakCount = 3;

struct HostFEStateMountainRoom {
    std::uint32_t magic = kMountainMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected_peak = 0;
    int initialized = 0;
};

HostFEStateMountainRoom* as_room(void* self) {
    auto* room = static_cast<HostFEStateMountainRoom*>(self);
    if (!room || room->magic != kMountainMagic) {
        return nullptr;
    }
    return room;
}

int fe_mountain_room_peak_count() { return kPeakCount; }

int fe_mountain_room_get_peak(void* self) {
    auto* room = as_room(self);
    return room ? room->selected_peak : 0;
}

void fe_mountain_room_set_peak(void* self, int index) {
    auto* room = as_room(self);
    if (!room) {
        return;
    }
    if (index < 0) {
        index = 0;
    }
    if (index >= kPeakCount) {
        index = kPeakCount - 1;
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
    auto* room = host::as_room(self);
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

void cFEStateMountainRoom_onCreateScreen(void* self) {
    auto* room = host::as_room(self);
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
    auto* room = host::as_room(self);
    if (!room) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    room->initialized = 0;
    host::host_log("fe", "cFEStateMountainRoom_onDestroyScreen");
}

void cFEStateMountainRoom_onUpdate(void* self) {
    auto* room = host::as_room(self);
    if (!room || !room->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    room->frame.time_sec += host::fe_frame_dt();
    host::build_mountain_room_screen(
        room->frame, viewport, room->frame.time_sec, room->selected_peak, host::kPeakCount);
    cVisualEffectsMainMenu_render(room->visual_fx, &room->frame);
}

void cFEStateMountainRoom_onWidgetCreate(void* self, void* widget) {
    (void)self;
    (void)widget;
    host::host_log("fe", "cFEStateMountainRoom_onWidgetCreate (retail 0x1D2AD0, stub)");
}

void cFEStateMountainRoom_onWidgetEvent(void* self, void* widget, int event) {
    (void)self;
    (void)widget;
    (void)event;
    host::host_log("fe", "cFEStateMountainRoom_onWidgetEvent (retail 0x1D2F68, stub)");
}

} // extern "C"
