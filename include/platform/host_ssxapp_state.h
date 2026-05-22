#ifndef PLATFORM_HOST_SSXAPP_STATE_H
#define PLATFORM_HOST_SSXAPP_STATE_H

#include <cstdint>

namespace host {

constexpr std::uint32_t kSSXAppMagic = 0x53535841u; /* "SSXA" */

struct HostSSXApp {
    std::uint32_t magic = kSSXAppMagic;
    int initialized = 0;
    int locale_ready = 0;
    int input_ready = 0;
    void* input_map = nullptr;
    void* crowd_anim = nullptr;
    void* game = nullptr;
    int pending_track = 0;
};

HostSSXApp* ssxapp_as_state(void* self);

} // namespace host

#endif /* PLATFORM_HOST_SSXAPP_STATE_H */
