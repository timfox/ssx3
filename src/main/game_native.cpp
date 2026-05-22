// Retail cGame @ 0x22EBC8+ (early segment) — native load bridge until game.cpp is decompiled.

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_game_state.h"
#include "platform/host_object.h"
#include "platform/host_game_mode.h"
#include "platform/host_log.h"
#include "platform/host_rider.h"
#include "platform/host_rider_state.h"
#include "platform/host_world.h"
#include "platform/host_world_state.h"

#include <cstdio>

extern "C" {
#endif

namespace {

void game_teardown_subsystems(host::HostGame* game) {
    if (!game) {
        return;
    }
    if (game->view_man) {
        host_game_view_destroy(game->view_man);
        game->view_man = nullptr;
    }
    if (game->rider) {
        host_rider_destroy(game->rider);
        game->rider = nullptr;
    }
    if (game->stream_man) {
        host_stream_man_destroy(game->stream_man);
        game->stream_man = nullptr;
    }
    if (game->world_cache) {
        host_world_cache_destroy(game->world_cache);
        game->world_cache = nullptr;
    }
    if (game->world_view) {
        host_world_view_destroy(game->world_view);
        game->world_view = nullptr;
    }
    if (game->world) {
        host_world_destroy(game->world);
        game->world = nullptr;
    }
    game->in_race = 0;
    game->race_ticks = 0;
}

bool game_ensure_subsystems(host::HostGame* game) {
    if (!game) {
        return false;
    }
    if (!game->world) {
        game->world = host_world_create();
    }
    if (!game->world_view) {
        game->world_view = host_world_view_create(4);
    }
    if (!game->world_cache) {
        game->world_cache = host_world_cache_create();
    }
    if (!game->stream_man) {
        game->stream_man = host_stream_man_create();
    }
    if (!game->view_man) {
        game->view_man = host_game_view_create();
    }
    return game->world && game->world_view && game->world_cache && game->stream_man && game->view_man;
}

} // namespace

void* cGame_cGame(void* self) {
    auto* game = host::game_as_state(self);
    if (!game) {
        return self;
    }
    host::host_log("game", "cGame_cGame (native @ 0x22EBC8 path)");
    if (game->world || game->view_man || game->rider) {
        game_teardown_subsystems(game);
    }
    (void)game_ensure_subsystems(game);
    if (game->view_man) {
        cGameViewMan_cGameViewMan(game->view_man);
    }
    if (game->stream_man) {
        cStreamMan_cStreamMan(game->stream_man);
    }
    return self;
}

int cGame_load(void* self, int mode) {
    auto* game = host::game_as_state(self);
    if (!game || !game_ensure_subsystems(game)) {
        return 0;
    }
    game->load_mode = mode;
    game->map_id = mode;
    game->loaded = 0;
    game->in_race = 0;

    char log_buf[96];
    std::snprintf(log_buf, sizeof(log_buf), "cGame_load mode=%d (retail 0x22EBC8)", mode);
    host::host_log("game", log_buf);

    cWorld_cWorld(game->world);
    cWorldCache_init(game->world_cache, game->world);

    void* gm = cGameModeMan_getGM();
    if (!cGameModeMan_initGameMode(gm, mode)) {
        return 0;
    }

    if (cWorld_resetMap(game->world, game->map_id) != 0) {
        return 0;
    }

    game->loaded = 1;
    return game->loaded;
}

int cGame_loadTrack(void* self, int track_id) {
    auto* game = host::game_as_state(self);
    if (!game || !game_ensure_subsystems(game)) {
        return 0;
    }
    game->track_id = track_id;
    game->map_id = track_id;

    char log_buf[96];
    std::snprintf(log_buf, sizeof(log_buf), "cGame_loadTrack id=%d (retail 0x22F6B0)", track_id);
    host::host_log("game", log_buf);

    if (cWorld_resetMap(game->world, track_id) != 0) {
        return 0;
    }

    host_instance_man_on_track_load(track_id);

    cSectionMan_setSky(0, track_id);
    if (cWorldCache_activateSectionMem(game->world_cache, game->world, 0) != 0) {
        return 0;
    }

    auto* view = host::world_view_as_state(game->world_view);
    if (view) {
        view->loaded_flags |= 1;
    }

    if (!game->rider) {
        game->rider = host_rider_create();
    }
    if (game->rider) {
        cRider_initOnce(game->rider);
        auto* rider = host::rider_as_state(game->rider);
        if (rider) {
            rider->initialized = 1;
        }
        host::host_log("game", "cRider_initOnce (retail 0x11C230, native)");
    }

    game->loaded = 1;
    game->in_race = 1;
    game->race_ticks = 0;
    return 1;
}

void cGame_exit(void* self) {
    auto* game = host::game_as_state(self);
    if (!game) {
        return;
    }
    host::host_log("game", "cGame_exit (retail 0x230488)");
    game->loaded = 0;
    game_teardown_subsystems(game);
}

int cGameModeHandler_run(void* handler, int mode) {
    auto* game = host::game_as_state(handler);
    if (!game || !game->in_race) {
        return mode;
    }
    host_game_tick(game);
    return mode;
}

void host_game_tick(void* self) {
    auto* game = host::game_as_state(self);
    if (!game || !game->in_race || !game->view_man) {
        return;
    }
    ++game->race_ticks;
    cGameViewMan_updateAll(game->view_man);

    if ((game->race_ticks % 120) == 0) {
        char log_buf[64];
        std::snprintf(log_buf, sizeof(log_buf), "race tick %d (mode %d)", game->race_ticks, game->load_mode);
        host::host_log("game", log_buf);
    }
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
