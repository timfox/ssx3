#include "platform/host_ssxapp.h"

#include "platform/host_game.h"
#include "platform/host_input.h"
#include "platform/host_ssxapp_state.h"

extern "C" {

void* host_ssxapp_create(void) {
    return new host::HostSSXApp();
}

void host_ssxapp_destroy(void* self) {
    auto* app = host::ssxapp_as_state(self);
    if (!app) {
        return;
    }
    if (app->input_map) {
        host_input_map_destroy(app->input_map);
        app->input_map = nullptr;
    }
    if (app->game) {
        host_game_destroy(app->game);
        app->game = nullptr;
    }
    app->crowd_anim = nullptr;
    delete app;
}

} // extern "C"

namespace host {

HostSSXApp* ssxapp_as_state(void* self) {
    auto* app = static_cast<HostSSXApp*>(self);
    if (!app || app->magic != kSSXAppMagic) {
        return nullptr;
    }
    return app;
}

} // namespace host
