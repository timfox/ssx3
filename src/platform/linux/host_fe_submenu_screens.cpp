#include "platform/host_fe_submenu.h"

#include "platform/host_fe_submenu_state.h"
#include "platform/host_fe_context.h"
#include "platform/host_fe_strings.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdio>
#include <cstring>

namespace host {

namespace {

const char* submenu_title_for(HostFESubmenuScreen* screen) {
    if (!screen) {
        return "";
    }
    if (screen->title_buf[0]) {
        return screen->title_buf;
    }
    switch (screen->magic) {
    case kFEPreviewsMagic:
        return fe_previews_screen_title();
    case kFEMultiplayMagic:
        return fe_multiplay_screen_title();
    case kFESingleEventMagic:
        return fe_single_event_screen_title();
    default:
        return "";
    }
}

const char* submenu_label_for(std::uint32_t magic, int index) {
    switch (magic) {
    case kFEPreviewsMagic:
        return fe_previews_label(index);
    case kFEMultiplayMagic:
        return fe_multiplay_label(index);
    case kFESingleEventMagic:
        return fe_single_event_label(index);
    case kFEPeakRoomMagic:
        return fe_peak_room_label(index);
    default:
        return "?";
    }
}

const char* submenu_description_for(std::uint32_t magic, int index) {
    switch (magic) {
    case kFEPreviewsMagic:
        return fe_previews_description(index);
    case kFEMultiplayMagic:
        return fe_multiplay_description(index);
    case kFESingleEventMagic:
        return fe_single_event_description(index);
    case kFEPeakRoomMagic:
        return fe_peak_room_description(index);
    default:
        return "";
    }
}

} // namespace

HostFESubmenuScreen* fe_submenu_as_state(void* self, std::uint32_t magic) {
    auto* screen = static_cast<HostFESubmenuScreen*>(self);
    if (!screen || screen->magic != magic) {
        return nullptr;
    }
    return screen;
}

void fe_submenu_navigate(HostFESubmenuScreen* screen, int delta) {
    if (!screen || screen->item_count <= 0) {
        return;
    }
    int next = screen->selected + delta;
    if (next < 0) {
        next = 0;
    }
    if (next >= screen->item_count) {
        next = screen->item_count - 1;
    }
    screen->selected = next;
}

void fe_submenu_on_create_screen(HostFESubmenuScreen* screen,
                                 const char* log_label,
                                 const char* retail_addr) {
    if (!screen) {
        return;
    }
    char log_buf[160];
    std::snprintf(log_buf, sizeof(log_buf), "%s (retail %s)", log_label, retail_addr);
    host_log("fe", log_buf);
    if (!screen->visual_fx) {
        screen->visual_fx = host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(screen->visual_fx, nullptr);
    screen->selected = 0;
    screen->initialized = 1;
}

void fe_submenu_on_destroy_screen(HostFESubmenuScreen* screen) {
    if (!screen) {
        return;
    }
    host_visualfx_mainmenu_set_active(nullptr);
    screen->initialized = 0;
}

void fe_submenu_on_update_list(HostFESubmenuScreen* screen) {
    if (!screen || !screen->initialized || screen->item_count <= 0) {
        return;
    }

    const MenuViewport viewport =
        compute_title_viewport(fe_frame_width(), fe_frame_height());

    const char* labels[8]{};
    const char* descriptions[8]{};
    for (int i = 0; i < screen->item_count && i < 8; ++i) {
        labels[i] = submenu_label_for(screen->magic, i);
        descriptions[i] = submenu_description_for(screen->magic, i);
    }

    screen->frame.time_sec += fe_frame_dt();
    build_fe_list_screen(screen->frame,
                         viewport,
                         screen->frame.time_sec,
                         submenu_title_for(screen),
                         labels,
                         descriptions,
                         screen->item_count,
                         screen->selected);
    cVisualEffectsMainMenu_render(screen->visual_fx, &screen->frame);
}

HostFESubmenuScreen* fe_submenu_create(std::uint32_t magic, const char* asset_dir, int peak_index) {
    auto* screen = new HostFESubmenuScreen();
    screen->magic = magic;
    screen->peak_index = peak_index;
    switch (magic) {
    case kFEPreviewsMagic:
        screen->item_count = fe_previews_item_count();
        break;
    case kFEMultiplayMagic:
        screen->item_count = fe_multiplay_item_count();
        break;
    case kFESingleEventMagic:
        screen->item_count = fe_single_event_item_count();
        break;
    case kFEPeakRoomMagic:
        screen->item_count = fe_peak_room_item_count();
        std::snprintf(screen->title_buf,
                      sizeof(screen->title_buf),
                      "%s — %s",
                      fe_mountain_peak_label(peak_index),
                      fe_string_mountain_select_peak());
        break;
    default:
        screen->item_count = 0;
        break;
    }
    if (asset_dir) {
        std::strncpy(screen->asset_dir, asset_dir, sizeof(screen->asset_dir) - 1);
    }
    screen->visual_fx = host_visualfx_mainmenu_create();
    return screen;
}

void fe_submenu_destroy(void* self, std::uint32_t magic) {
    auto* screen = fe_submenu_as_state(self, magic);
    if (!screen) {
        return;
    }
    if (screen->visual_fx) {
        host_visualfx_mainmenu_set_active(nullptr);
        host_visualfx_mainmenu_destroy(screen->visual_fx);
        screen->visual_fx = nullptr;
    }
    delete screen;
}

int fe_previews_get_selected(void* self) {
    auto* screen = fe_submenu_as_state(self, kFEPreviewsMagic);
    return screen ? screen->selected : 0;
}

void fe_previews_navigate(void* self, int delta) {
    fe_submenu_navigate(fe_submenu_as_state(self, kFEPreviewsMagic), delta);
}

int fe_multiplay_get_selected(void* self) {
    auto* screen = fe_submenu_as_state(self, kFEMultiplayMagic);
    return screen ? screen->selected : 0;
}

void fe_multiplay_navigate(void* self, int delta) {
    fe_submenu_navigate(fe_submenu_as_state(self, kFEMultiplayMagic), delta);
}

int fe_single_event_get_selected(void* self) {
    auto* screen = fe_submenu_as_state(self, kFESingleEventMagic);
    return screen ? screen->selected : 0;
}

void fe_single_event_navigate(void* self, int delta) {
    fe_submenu_navigate(fe_submenu_as_state(self, kFESingleEventMagic), delta);
}

int fe_peak_room_get_selected(void* self) {
    auto* screen = fe_submenu_as_state(self, kFEPeakRoomMagic);
    return screen ? screen->selected : 0;
}

void fe_peak_room_navigate(void* self, int delta) {
    fe_submenu_navigate(fe_submenu_as_state(self, kFEPeakRoomMagic), delta);
}

int fe_peak_room_get_peak(void* self) {
    auto* screen = fe_submenu_as_state(self, kFEPeakRoomMagic);
    return screen ? screen->peak_index : 0;
}

} // namespace host

extern "C" {

void* host_fe_previews_create(const char* asset_dir) {
    return host::fe_submenu_create(host::kFEPreviewsMagic, asset_dir, 0);
}

void host_fe_previews_destroy(void* self) {
    host::fe_submenu_destroy(self, host::kFEPreviewsMagic);
}

void* host_fe_multiplay_create(const char* asset_dir) {
    return host::fe_submenu_create(host::kFEMultiplayMagic, asset_dir, 0);
}

void host_fe_multiplay_destroy(void* self) {
    host::fe_submenu_destroy(self, host::kFEMultiplayMagic);
}

void* host_fe_single_event_create(const char* asset_dir) {
    return host::fe_submenu_create(host::kFESingleEventMagic, asset_dir, 0);
}

void host_fe_single_event_destroy(void* self) {
    host::fe_submenu_destroy(self, host::kFESingleEventMagic);
}

void* host_fe_peak_room_create(const char* asset_dir, int peak_index) {
    return host::fe_submenu_create(host::kFEPeakRoomMagic, asset_dir, peak_index);
}

void host_fe_peak_room_destroy(void* self) {
    host::fe_submenu_destroy(self, host::kFEPeakRoomMagic);
}

} // extern "C"
