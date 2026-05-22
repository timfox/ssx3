#include "platform/host_game.h"

#include "platform/host_game_state.h"
#include "platform/host_world.h"
#include "platform/host_game_mode.h"

namespace host {

HostGame* game_as_state(void* self) {
    auto* game = static_cast<HostGame*>(self);
    if (!game || game->magic != kHostGameMagic) {
        return nullptr;
    }
    return game;
}

} // namespace host

extern "C" {

void cGame_exit(void* self);

void* host_game_create(void) {
    auto* game = new host::HostGame();
    game->world = nullptr;
    game->world_view = nullptr;
    game->world_cache = nullptr;
    game->stream_man = nullptr;
    game->view_man = nullptr;
    game->rider = nullptr;
    return game;
}

void host_game_destroy(void* self) {
    if (self) {
        cGame_exit(self);
    }
    delete host::game_as_state(self);
}

} // extern "C"
