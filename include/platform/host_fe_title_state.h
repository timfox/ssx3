#ifndef PLATFORM_HOST_FE_TITLE_STATE_H
#define PLATFORM_HOST_FE_TITLE_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEStateTitleMagic = 0x46455449u; /* "FETI" */

struct HostFEStateTitle {
    std::uint32_t magic = kFEStateTitleMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    float menu_time = 0.0f;
    int initialized = 0;
};

HostFEStateTitle* fe_title_as_state(void* self);

} // namespace host

#endif /* PLATFORM_HOST_FE_TITLE_STATE_H */
