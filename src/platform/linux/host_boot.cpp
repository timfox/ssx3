#include "platform/host_boot.h"

#include "retail/appman_layout.h"
#include "platform/host_abi.h"
#include "platform/host_bigfile.h"
#include "platform/host_fe.h"
#include "platform/host_fe_title.h"
#include "platform/host_input.h"
#include "platform/host_log.h"
#include "platform/host_pad.h"
#include "platform/host_fe_assets.h"
#include "platform/host_cfelocale.h"
#include "platform/host_retail.h"
#include "platform/host_ssxapp.h"
#include "platform/host_game.h"
#include "platform/host_game_state.h"
#include "platform/host_ssxapp_state.h"
#include "platform/host_gameplay.h"
#include "platform/host_renderer.h"
#include "platform/host_object.h"
#include "platform/host_track_map.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

namespace host {
namespace {

/* Retail main() after systemInit: cMemMan_alloc(0x142c0, …). */
constexpr unsigned long kMainHeapBytes = 0x142c0u;

/* VMAs from config/symbol_addrs.txt (EE main segment @ 0x100000). */
constexpr unsigned long kAddrRetailPreinit = 0x0040fc90u;
constexpr unsigned long kAddrStaticInitLoop = 0x0031af50u;
constexpr unsigned long kAddrModuleInit = 0x0031b098u;

struct AppContext {
    char pad_0[4];
    void* next_module; /* +4  cAppMan_setNextModule */
    int argc;          /* +8  cAppMan_run */
    char** argv;       /* +12 (retail also touches argv object) */
    int tick_budget;   /* +16 cAppMan_run writes 60 */
    char pad_20[0x5c - 20];
    void* module_table; /* +0x5c */
    void* ssx_app;
    void* heap_tag;
    void* globals_ptr;
};

AppContext* g_app = nullptr;
void* g_preinit_done = nullptr;

void retail_preinit(void* a0, void* a1) {
    (void)a0;
    (void)a1;
    if (!g_preinit_done) {
        host_log("boot", "retail_preinit (retail 0x40fc90)");
        g_preinit_done = reinterpret_cast<void*>(1);
    }
}

/* Stand-in for __sti / static-init loop at retail 0x31AF50 (49 ctor slots). */
void retail_run_static_init(void* app_base) {
    (void)app_base;
    host_log("boot", "retail_run_static_init (retail 0x31AF50, 49 ctor slots)");
}

/* Stand-in for per-module init invoked from main at retail 0x31B098. */
void retail_module_init(void* app_base, int module_index) {
    host_retail_module_init(app_base, module_index);
}

} // namespace
} // namespace host

extern "C" {

void host_app_request_quit(void);
int host_app_tick_count(void);
void host_app_reset_quit(void);

void systemInit(void* a0, void* a1, int a2, void* a3) {
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
    cMemMan_initialize(nullptr, nullptr, 0, nullptr);
    ::host::host_log("boot", "systemInit");
}

void cExecutionMan_halt(void* ctx) {
    ::host::host_log("boot", "cExecutionMan_halt (execution manager setup)");
    if (!ctx) {
        return;
    }
    auto* app = static_cast<::host::AppContext*>(ctx);
    app->tick_budget = 60;
}

void host_appman_main_loop(void* ctx) {
    auto* app = static_cast<::host::AppContext*>(ctx);
    if (!app) {
        return;
    }

    host_app_reset_quit();
    ::host::pad_poll();

    if (::host::run_boot_appman_vulkan_session(app->ssx_app)) {
        ::host::pad_shutdown();
        return;
    }

    ::host::host_log("boot", "Vulkan unavailable — headless cAppMan tick loop");
    while (host_app_tick()) {
        if (app->ssx_app) {
            cSSXApp_preUpdate(app->ssx_app);
            host_retail_tick(app->ssx_app);
        }
        if (host_app_tick_count() > 3600) {
            break;
        }
    }
    ::host::pad_shutdown();
}

void host_appman_run(void* ctx, int argc, char** argv) {
    auto* app = static_cast<::host::AppContext*>(ctx);
    if (!app) {
        return;
    }
    app->argc = argc;
    app->argv = argv;
    app->tick_budget = 60;
    ::host::host_log("boot", "cAppMan_run → cAppMan_mainLoop (Vulkan)");
    cAppMan_mainLoop(ctx);
}

} // extern "C"

namespace host {

bool run_retail_boot_init(int argc, char** argv) {
    std::cout << "[boot]    retail boot init (host HAL, not MIPS)\n";
    std::cout << "[boot]    VMAs: preinit=0x" << std::hex << kAddrRetailPreinit
              << " static_init=0x" << kAddrStaticInitLoop << " module_init=0x" << kAddrModuleInit
              << " title=0x194778" << std::dec << '\n';

    host_app_reset_quit();

    retail_preinit(reinterpret_cast<void*>(static_cast<intptr_t>(argc)),
                   reinterpret_cast<void*>(argv));

    systemInit(nullptr, nullptr, 0, nullptr);

    void* heap_tag = reinterpret_cast<void*>(0x49dae8u);
    void* block = cMemMan_alloc(kMainHeapBytes, heap_tag, 0, nullptr);
    if (!block) {
        std::cerr << "[boot]    cMemMan_alloc failed; aborting\n";
        return false;
    }

    auto* app = static_cast<AppContext*>(block);
    std::memset(app, 0, sizeof(AppContext));
    app->heap_tag = heap_tag;
    app->globals_ptr = reinterpret_cast<void*>(0x49db68u);
    app->argc = argc;
    app->argv = argv;
    app->tick_budget = 60;
    g_app = app;

    host_object_interface_init();

    retail_run_static_init(app);
    for (int module = 0; module < 12; ++module) {
        retail_module_init(app, module);
    }

    app->ssx_app = host_ssxapp_create();
    if (app->ssx_app) {
        cSSXApp_cSSXApp(app->ssx_app);
        if (cSSXApp_init(app->ssx_app)) {
            host_log("boot", "cSSXApp_init OK (persists for cAppMan_mainLoop)");
        }
    }

    HostBigFile boot_archive{};
    cBigFile_cBigFile(&boot_archive);
    if (cBigFile_open(&boot_archive, "data/audio/audio.big")) {
        host_log("boot", "cBigFile_open data/audio/audio.big OK");
        char header[16]{};
        const unsigned long n = cBigFile_read(&boot_archive, "data\\audio\\BC_AmbLoop1.bnk", header, sizeof(header));
        if (n > 0) {
            host_log("boot", "cBigFile_read BC_AmbLoop1 header OK");
        }
    }
    cBigFile_close(&boot_archive);

    void* fe_title = host_fe_title_create(host::fe_menu_asset_dir().c_str());
    if (fe_title) {
        cFEStateTitle_onCreateScreen(fe_title);
        cFEStateTitle_onDestroyScreen(fe_title);
        host_fe_title_destroy(fe_title);
    }

    ::host::pad_init();
    cExecutionMan_halt(app);
    return true;
}

void run_retail_boot_shutdown(void) {
    if (g_app) {
        if (g_app->ssx_app) {
            host_ssxapp_destroy(g_app->ssx_app);
            g_app->ssx_app = nullptr;
        }
        cMemMan_free(g_app);
        g_app = nullptr;
    }
}

void* host_boot_ssx_app(void) {
    return g_app ? g_app->ssx_app : nullptr;
}

void* host_boot_active_game(void) {
    if (!g_app || !g_app->ssx_app) {
        return nullptr;
    }
    auto* ssx = host::ssxapp_as_state(g_app->ssx_app);
    return ssx ? ssx->game : nullptr;
}

int host_boot_race_map_id(void) {
    auto* game = host::game_as_state(host_boot_active_game());
    return game ? game->map_id : 0;
}

int host_boot_start_race_load_mapped(int load_mode, int track_id) {
    void* ssx = host_boot_ssx_app();
    if (!ssx) {
        return 0;
    }
    if (track_id < 0) {
        track_id = load_mode;
    }
    track_log_disc_availability(track_id);

    char log_buf[128];
    std::snprintf(log_buf,
                  sizeof(log_buf),
                  "race load mode=%d track=%d (%s)",
                  load_mode,
                  track_id,
                  track_map_label(track_id));
    host_log("boot", log_buf);

    if (!cSSXApp_startGameLoad(ssx, load_mode)) {
        return 0;
    }
    if (auto* app = host::ssxapp_as_state(ssx)) {
        app->pending_track = track_id;
    }
    return cSSXApp_initload(ssx) ? 1 : 0;
}

int host_boot_start_race_load(int track_id) {
    return host_boot_start_race_load_mapped(track_id, track_id);
}

bool run_game_boot_chain(int argc, char** argv) {
    if (!run_retail_boot_init(argc, argv)) {
        return false;
    }
    if (!g_app) {
        return false;
    }

    RendererConfig config{};
    config.title = "SSX3";
    config.width = 1280;
    config.height = 720;
    config.vsync = true;
    config.shader_dir = "obj/host/shaders";
    config.upscale = UpscaleBackend::Direct;

    bool ok = false;
    if (run_boot_fe_vulkan_session(config)) {
        ok = true;
        host_log("boot", "cAppMan_mainLoop — retail FE session finished");
    } else {
        host_log("boot", "FE unavailable — terrain cAppMan_mainLoop");
        cAppMan_run(g_app, argc, argv);
        ok = true;
    }

    run_retail_boot_shutdown();
    host_log("boot", "boot chain finished");
    return ok;
}

} // namespace host
