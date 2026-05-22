#ifndef PLATFORM_HOST_FE_OPTIONS_STATE_H
#define PLATFORM_HOST_FE_OPTIONS_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEOptionsMagic = 0x46454F50u; /* "FEOP" */

struct HostFEStateOptions {
    std::uint32_t magic = kFEOptionsMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int initialized = 0;
};

HostFEStateOptions* fe_options_as_state(void* self);
int fe_options_get_selected(void* self);
void fe_options_navigate(void* self, int delta);
bool fe_options_selection_is_back(void* self);

} // namespace host

#endif /* PLATFORM_HOST_FE_OPTIONS_STATE_H */
