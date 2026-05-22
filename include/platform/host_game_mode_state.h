#ifndef PLATFORM_HOST_GAME_MODE_STATE_H
#define PLATFORM_HOST_GAME_MODE_STATE_H

#include <cstdint>

namespace host {

constexpr std::uint32_t kHostGameModeManMagic = 0x474D4D4Du; /* "GMMM" */
constexpr std::uint32_t kHostGameViewManMagic = 0x4756564Du; /* "GVVM" */

struct HostGameModeMan {
    std::uint32_t magic = kHostGameModeManMagic;
    int current_mode = 0;
    int heat_restarts = 0;
    int initialized = 0;
};

struct HostGameViewMan {
    std::uint32_t magic = kHostGameViewManMagic;
    int frame = 0;
    int active_views = 1;
};

HostGameModeMan* game_mode_man_as_state(void* self);
HostGameViewMan* game_view_man_as_state(void* self);

} // namespace host

#endif /* PLATFORM_HOST_GAME_MODE_STATE_H */
