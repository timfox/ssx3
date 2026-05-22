#ifndef PLATFORM_HOST_FE_TRANSITION_STATE_H
#define PLATFORM_HOST_FE_TRANSITION_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFETransitionMagic = 0x46455452u; /* "FETR" */

struct HostFETransition {
    std::uint32_t magic = kFETransitionMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int initialized = 0;
    float time_sec = 0.0f;
};

HostFETransition* fe_transition_as_state(void* self);

} // namespace host

#endif /* PLATFORM_HOST_FE_TRANSITION_STATE_H */
