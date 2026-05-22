#ifndef PLATFORM_HOST_RIDER_STATE_H
#define PLATFORM_HOST_RIDER_STATE_H

#include <cstdint>

namespace host {

constexpr std::uint32_t kHostRiderMagic = 0x52494452u; /* "RIDR" */

struct HostRider {
    std::uint32_t magic = kHostRiderMagic;
    int initialized = 0;
    float mass = 75.0f;
};

HostRider* rider_as_state(void* self);

} // namespace host

#endif /* PLATFORM_HOST_RIDER_STATE_H */
