#ifndef PLATFORM_HOST_WORLD_STATE_H
#define PLATFORM_HOST_WORLD_STATE_H

#include <cstdint>

namespace host {

constexpr std::uint32_t kHostWorldMagic = 0x574C4449u; /* "WLDI" */
constexpr std::uint32_t kHostWorldViewMagic = 0x574C4456u; /* "WLDV" */
constexpr std::uint32_t kHostWorldCacheMagic = 0x574C4443u; /* "WLDC" */
constexpr std::uint32_t kHostStreamManMagic = 0x53544D4eu; /* "STMN" */

struct HostWorld {
    std::uint32_t magic = kHostWorldMagic;
    int map_id = -1;
    int reset_count = 0;
    int section_count = 4;
};

/* Retail cWorldView layout (from rom21 @ 0x3A9800 / 0x3A9858). */
struct HostWorldView {
    std::uint32_t magic = kHostWorldViewMagic;
    int guard = 0;
    int num_sections = 4;
    int loaded_flags = 0;
};

struct HostWorldCache {
    std::uint32_t magic = kHostWorldCacheMagic;
    void* world = nullptr;
    int active_section = -1;
};

struct HostStreamMan {
    std::uint32_t magic = kHostStreamManMagic;
    int stream_slots = 2;
};

HostWorld* world_as_state(void* self);
HostWorldView* world_view_as_state(void* view);
HostWorldCache* world_cache_as_state(void* cache);
HostStreamMan* stream_man_as_state(void* self);

} // namespace host

#endif /* PLATFORM_HOST_WORLD_STATE_H */
