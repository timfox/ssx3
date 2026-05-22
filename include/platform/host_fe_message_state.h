#ifndef PLATFORM_HOST_FE_MESSAGE_STATE_H
#define PLATFORM_HOST_FE_MESSAGE_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEMessageMagic = 0x46454D47u; /* "FEMG" */

struct HostFEMessageScreen {
    std::uint32_t magic = kFEMessageMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    char title[128]{};
    char body[512]{};
    int initialized = 0;
};

HostFEMessageScreen* fe_message_as_state(void* self);
void fe_message_set_text(HostFEMessageScreen* screen, const char* title, const char* body);
void fe_message_on_create_screen(HostFEMessageScreen* screen,
                                 const char* log_label,
                                 const char* retail_addr);
void fe_message_on_destroy_screen(HostFEMessageScreen* screen);
void fe_message_on_update_screen(HostFEMessageScreen* screen);

} // namespace host

#endif /* PLATFORM_HOST_FE_MESSAGE_STATE_H */
