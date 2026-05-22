#include "platform/host_fe_message.h"

#include "platform/host_fe_message_state.h"
#include "platform/host_fe_strings.h"
#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdio>
#include <cstring>

namespace host {

HostFEMessageScreen* fe_message_as_state(void* self) {
    auto* screen = static_cast<HostFEMessageScreen*>(self);
    if (!screen || screen->magic != kFEMessageMagic) {
        return nullptr;
    }
    return screen;
}

void fe_message_set_text(HostFEMessageScreen* screen, const char* title, const char* body) {
    if (!screen) {
        return;
    }
    if (title) {
        std::strncpy(screen->title, title, sizeof(screen->title) - 1);
    }
    if (body) {
        std::strncpy(screen->body, body, sizeof(screen->body) - 1);
    }
}

void fe_message_on_create_screen(HostFEMessageScreen* screen,
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
    screen->initialized = 1;
}

void fe_message_on_destroy_screen(HostFEMessageScreen* screen) {
    if (!screen) {
        return;
    }
    host_visualfx_mainmenu_set_active(nullptr);
    screen->initialized = 0;
}

void fe_message_on_update_screen(HostFEMessageScreen* screen) {
    if (!screen || !screen->initialized) {
        return;
    }

    const MenuViewport viewport =
        compute_title_viewport(fe_frame_width(), fe_frame_height());

    screen->frame.time_sec += fe_frame_dt();
    build_fe_info_screen(
        screen->frame, viewport, screen->frame.time_sec, screen->title, screen->body);
}

} // namespace host

extern "C" {

void* host_fe_message_create(const char* asset_dir, const char* title, const char* body) {
    auto* screen = new host::HostFEMessageScreen();
    if (asset_dir) {
        std::strncpy(screen->asset_dir, asset_dir, sizeof(screen->asset_dir) - 1);
    }
    host::fe_message_set_text(screen, title, body);
    screen->visual_fx = host::host_visualfx_mainmenu_create();
    return screen;
}

void host_fe_message_destroy(void* self) {
    auto* screen = host::fe_message_as_state(self);
    if (!screen) {
        return;
    }
    if (screen->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(screen->visual_fx);
        screen->visual_fx = nullptr;
    }
    delete screen;
}

void* host_fe_online_create(const char* asset_dir) {
    return host_fe_message_create(
        asset_dir, host::fe_online_screen_title(), host::fe_online_body_text());
}

void host_fe_online_destroy(void* self) {
    host_fe_message_destroy(self);
}

void* host_fe_options_controller_create(const char* asset_dir) {
    return host_fe_message_create(
        asset_dir, host::fe_options_label(2), host::fe_options_controller_body());
}

void host_fe_options_controller_destroy(void* self) {
    host_fe_message_destroy(self);
}

void* host_fe_options_saveload_create(const char* asset_dir) {
    return host_fe_message_create(
        asset_dir, host::fe_options_label(3), host::fe_options_saveload_body());
}

void host_fe_options_saveload_destroy(void* self) {
    host_fe_message_destroy(self);
}

void* host_fe_preview_detail_create(const char* asset_dir, int preview_index) {
    return host_fe_message_create(asset_dir,
                                  host::fe_previews_label(preview_index),
                                  host::fe_previews_description(preview_index));
}

void host_fe_preview_detail_destroy(void* self) {
    host_fe_message_destroy(self);
}

void* host_fe_peak_event_stub_create(const char* asset_dir, int peak_index, int mode_index) {
    char body[320];
    std::snprintf(body,
                  sizeof(body),
                  "%s on %s — retail event/load flow not ported (cFEStateEventSelect / peak run).",
                  host::fe_peak_room_label(mode_index),
                  host::fe_mountain_peak_label(peak_index));
    char title[96];
    std::snprintf(title, sizeof(title), "%s", host::fe_mountain_peak_label(peak_index));
    return host_fe_message_create(asset_dir, title, body);
}

void host_fe_peak_event_stub_destroy(void* self) {
    host_fe_message_destroy(self);
}

} // extern "C"
