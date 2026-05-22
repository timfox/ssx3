#ifndef PLATFORM_HOST_GAME_STATE_H
#define PLATFORM_HOST_GAME_STATE_H

#include <cstdint>

namespace host {

constexpr std::uint32_t kHostGameMagic = 0x47534D47u; /* "GSMG" */

struct HostGame {
    std::uint32_t magic = kHostGameMagic;
    int load_mode = 0;
    int track_id = 0;
    int map_id = 0;
    int loaded = 0;
    int in_race = 0;
    int race_ticks = 0;
    void* world = nullptr;
    void* world_view = nullptr;
    void* world_cache = nullptr;
    void* stream_man = nullptr;
    void* view_man = nullptr;
    void* rider = nullptr;
};

HostGame* game_as_state(void* self);

} // namespace host

#endif /* PLATFORM_HOST_GAME_STATE_H */
