#include "common.h"

#ifdef SSX3_HOST
#include "platform/host_abi.h"
extern "C" {
#endif

/* Retail @ 0x217F00 — EE matching via src/mem/units/dcd10_cAppMan_mainLoop.c */
INCLUDE_ASM("dcd10/dcd10_cAppMan", cAppMan_mainLoop);

#ifdef SKIP_ASM
#ifdef SSX3_HOST
/* HAL implementation (SDL + SYNCTASK_run via host_retail_tick). */
extern "C" void host_appman_main_loop(void* ctx);

extern "C" void cAppMan_mainLoop(void* ctx) {
    host_appman_main_loop(ctx);
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif
