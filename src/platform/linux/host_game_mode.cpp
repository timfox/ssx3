#include "platform/host_game_mode.h"

#include "platform/host_game_mode_state.h"

namespace host {

HostGameModeMan* game_mode_man_as_state(void* self) {
    auto* gm = static_cast<HostGameModeMan*>(self);
    if (!gm || gm->magic != kHostGameModeManMagic) {
        return nullptr;
    }
    return gm;
}

HostGameViewMan* game_view_man_as_state(void* self) {
    auto* vm = static_cast<HostGameViewMan*>(self);
    if (!vm || vm->magic != kHostGameViewManMagic) {
        return nullptr;
    }
    return vm;
}

} // namespace host

extern "C" {

void* host_game_view_create(void) {
    return new host::HostGameViewMan();
}

void host_game_view_destroy(void* self) {
    delete host::game_view_man_as_state(self);
}

} // extern "C"
