// Readable retail boot/runtime helpers for the native port.
// PS2 objdiff uses src/mem/units/*.c (embedded retail opcodes).

#include "common.h"

#include "retail/appman_layout.h"

#ifdef SSX3_HOST
#include "platform/host_log.h"
#include "platform/host_synctask.h"

extern "C" {
#endif

/*
 * Retail SYNCTASK_init (rom21 @ 0x2E67D0):
 *   func_003E6448((void*)0x0051ED98, 0, 0x100);  // optimized memset @ 0x2E7448
 * EE objdiff: src/mem/units/rom21_SYNCTASK_init.c (embedded opcodes).
 * Native pool: retail_memset_u8 + host_synctask_init in synctask_pool.cpp.
 *
 * Retail cAppMan_run (dcd10 @ 0x217D60): stores argc/argv, runs inits, enters mainLoop.
 * Native: src/dcd10/cAppMan.cpp → host_appman_run in host_boot.cpp.
 */

/* App manager: next module pointer at +4 (see early asm call sites). */
void cAppMan_setNextModule(void* app, void* next_module) {
    if (app) {
        *(void**)((char*)app + kAppManNextModule) = next_module;
    }
}

void SYNCTASK_init(void) {
#ifdef SSX3_HOST
    host::host_log("sync", "SYNCTASK_init");
    host::host_synctask_init();
#endif
}

#ifdef SSX3_HOST
}
#endif
