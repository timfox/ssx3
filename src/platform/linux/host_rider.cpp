#include "platform/host_rider.h"

#include "platform/host_rider_state.h"

namespace host {

HostRider* rider_as_state(void* self) {
    auto* rider = static_cast<HostRider*>(self);
    if (!rider || rider->magic != kHostRiderMagic) {
        return nullptr;
    }
    return rider;
}

} // namespace host

extern "C" {

void* host_rider_create(void) {
    return new host::HostRider();
}

void host_rider_destroy(void* self) {
    auto* rider = host::rider_as_state(self);
    if (rider) {
        delete rider;
    }
}

} // extern "C"
