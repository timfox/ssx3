// Retail cSSXApp entry points @ 0x226830+ (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_cfelocale.h"
#include "platform/host_fe_locale.h"
#include "platform/host_input.h"
#include "platform/host_io.h"
#include "platform/host_log.h"
#include "platform/host_retail.h"
#include "platform/host_ssxapp_state.h"
#include "platform/host_game.h"
#include "platform/host_game_state.h"
#include "platform/host_game_mode.h"
#include "platform/host_ui_engine.h"

#include "visualfx/crowdrender2d.h"

#include <cstring>

extern "C" {
#endif

void cSSXApp_cSSXApp(void* self) {
    auto* app = host::ssxapp_as_state(self);
    if (!app) {
        return;
    }
    app->magic = host::kSSXAppMagic;
    app->initialized = 0;
    app->input_ready = 0;
    app->locale_ready = 0;
    app->input_map = nullptr;
    app->crowd_anim = nullptr;
    app->game = nullptr;
    app->pending_track = 0;
}

void cSSXApp_purge(void* self) {
    auto* app = host::ssxapp_as_state(self);
    if (!app) {
        return;
    }
    app->initialized = 0;
    app->input_ready = 0;
    app->locale_ready = 0;
    app->crowd_anim = nullptr;
    app->game = nullptr;
}

void cSSXApp__cSSXApp(void* self) {
    cSSXApp_purge(self);
}

int cSSXApp_loadInputMap(void* self, const char* path) {
    auto* app = host::ssxapp_as_state(self);
    if (!app) {
        return 0;
    }
    if (!app->input_map) {
        app->input_map = host_input_map_create();
    }
    if (!app->input_map) {
        return 0;
    }
    cInputMap_init(app->input_map);
    const char* map_path = path ? path : "data/config/input.map";
    if (!cInputMap_loadMapFile(app->input_map, map_path)) {
        return 0;
    }
    return cInputMap_compileMap(app->input_map) ? 1 : 0;
}

int cSSXApp_initLocale(void* self) {
    (void)self;
    host::fe_locale_init();

    static const char* kLocFiles[] = {
        "data/locale/cmnamer.loc",
        "data/locale/feamer.loc",
        "data/locale/fegerman.loc",
    };

    int loaded = 0;
    for (const char* loc_path : kLocFiles) {
        if (cFELocale_addFile(nullptr, loc_path)) {
            ++loaded;
        }
    }

    if (loaded > 0) {
        return 1;
    }
    const auto feamer = host::read_disc_file("data/locale/feamer.loc");
    return feamer && !feamer->empty() ? 1 : 0;
}

int cSSXApp_init(void* self) {
    auto* app = host::ssxapp_as_state(self);
    if (!app) {
        return 0;
    }
    if (app->initialized) {
        return 1;
    }

    if (!cSSXApp_loadInputMap(self, "data/config/input2.map") &&
        !cSSXApp_loadInputMap(self, "data/config/input.map")) {
        host::host_log("ssxapp", "cSSXApp_init: input map load failed");
        return 0;
    }
    app->input_ready = 1;

    cCrowdRender2D_init();
    app->crowd_anim = cCrowdRender2D_constructCrowdAnim2D(nullptr);

    if (!cSSXApp_initLocale(self)) {
        host::host_log("ssxapp", "cSSXApp_init: locale init failed");
    } else {
        app->locale_ready = 1;
    }

    void* ui = host_ui_engine_create();
    if (ui) {
        if (cUIEngine_loadFile(ui, "data/ui/fe.lui")) {
            (void)cUIEngine_loadFile(ui, "data/ui/fl.lui");
        }
        host_ui_engine_destroy(ui);
    }

    if (!app->game) {
        app->game = host_game_create();
        if (app->game) {
            cGame_cGame(app->game);
        }
    }

    app->initialized = 1;
    host::host_log("ssxapp", "cSSXApp_init OK");
    return 1;
}

void cSSXApp_preUpdate(void* self) {
    auto* app = host::ssxapp_as_state(self);
    if (app && app->game) {
        auto* game = host::game_as_state(app->game);
        if (game && game->in_race) {
            host_game_tick(app->game);
        }
    }
    host_retail_tick(self);
}

void cSSXApp_flush(void* self) {
    (void)self;
}

int cSSXApp_parseCommandLine(void* self, int argc, char** argv) {
    (void)self;
    (void)argc;
    (void)argv;
    return 1;
}

void cSSXApp_timerCallback(void* self) {
    (void)self;
}

int cSSXApp_startGameLoad(void* self, int mode) {
    auto* app = host::ssxapp_as_state(self);
    if (!app) {
        return 0;
    }
    if (!app->game) {
        app->game = host_game_create();
        if (app->game) {
            cGame_cGame(app->game);
        }
    }
    app->pending_track = mode;
    host::host_log("ssxapp", "cSSXApp_startGameLoad (retail 0x281D8)");
    if (!app->game) {
        return 0;
    }
    return cGame_load(app->game, mode) ? 1 : 0;
}

int cSSXApp_initload(void* self) {
    auto* app = host::ssxapp_as_state(self);
    if (!app || !app->game) {
        return 0;
    }
    host::host_log("ssxapp", "cSSXApp_initload (retail 0x282D0)");
    return cGame_loadTrack(app->game, app->pending_track);
}

void cSSXApp_loadexecpurge(void* self) {
    auto* app = host::ssxapp_as_state(self);
    if (!app) {
        return;
    }
    host::host_log("ssxapp", "cSSXApp_loadexecpurge (retail 0x28BB8)");
    if (app->game) {
        cGame_exit(app->game);
    }
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
