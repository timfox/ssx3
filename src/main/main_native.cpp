// Retail main() @ 0x31AF80 — native entry documents the boot chain.

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_boot.h"
#include "platform/host_log.h"

extern "C" {
#endif

/*
 * Retail main (NTSC-U 0x31AF80):
 *   cMemMan_alloc → systemInit → static ctors → module inits → cAppMan_run
 * Native: host::run_game_boot_chain mirrors this via HAL.
 */
int ssx3_retail_main(int argc, char** argv) {
    if (!host::run_game_boot_chain(argc, argv)) {
        host::host_log("main", "run_game_boot_chain failed");
        return 1;
    }
    return 0;
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
