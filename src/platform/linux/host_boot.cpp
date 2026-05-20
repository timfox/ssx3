#include "platform/host_boot.h"

#include "platform/host_abi.h"
#include "platform/host_log.h"

#include <SDL.h>

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
    void* heap_tag;
    int field_16;
    int argc;
    char** argv;
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
    (void)app_base;
    (void)module_index;
}

} // namespace
} // namespace host

extern "C" {

void host_app_request_quit(void);
int host_app_tick_count(void);

void systemInit(void* a0, void* a1, int a2, void* a3) {
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
    cMemMan_initialize(nullptr, nullptr, 0, nullptr);
    host::host_log("boot", "systemInit");
}

void cExecutionMan_halt(void* ctx) {
    host::host_log("boot", "cExecutionMan_halt (execution manager setup)");
    if (!ctx) {
        return;
    }
    auto* app = static_cast<host::AppContext*>(ctx);
    app->field_16 = 60;
}

void cAppMan_mainLoop(void* ctx) {
    auto* app = static_cast<host::AppContext*>(ctx);
    if (!app) {
        return;
    }

    host::host_log("boot", "cAppMan_mainLoop — host tick loop (Ctrl+C or close SDL window to stop)");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        while (host_app_tick()) {
            if (host_app_tick_count() > 3600) {
                break;
            }
        }
        return;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SSX3 host boot",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        448,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Quit();
        while (host_app_tick()) {
            if (host_app_tick_count() > 3600) {
                break;
            }
        }
        return;
    }

    bool running = true;
    while (running && host_app_tick()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                host_app_request_quit();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
                host_app_request_quit();
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void cAppMan_run(void* ctx, int argc, char** argv) {
    auto* app = static_cast<host::AppContext*>(ctx);
    if (!app) {
        return;
    }
    app->argc = argc;
    app->argv = argv;
    host::host_log("boot", "cAppMan_run → cAppMan_mainLoop");
    cAppMan_mainLoop(ctx);
}

} // extern "C"

namespace host {

bool run_game_boot_chain(int argc, char** argv) {
    std::cout << "[boot]    Phase 2: retail main() chain via host HAL\n";
    std::cout << "[boot]    retail VMAs: preinit=0x" << std::hex << kAddrRetailPreinit
              << " static_init=0x" << kAddrStaticInitLoop << " module_init=0x" << kAddrModuleInit
              << std::dec << '\n';

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
    g_app = app;

    retail_run_static_init(app);
    for (int module = 0; module < 12; ++module) {
        retail_module_init(app, module);
    }

    cExecutionMan_halt(app);
    cAppMan_run(app, argc, argv);

    cMemMan_free(block);
    g_app = nullptr;

    host_log("boot", "boot chain finished");
    return true;
}

} // namespace host
