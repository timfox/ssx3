#include "common.h"

#ifdef SSX3_HOST
#include "platform/host_abi.h"
extern "C" {
#endif

/* Retail @ 0x217D60 — EE matching: src/mem/units/dcd10_cAppMan_run.c */
INCLUDE_ASM("dcd10/dcd10_cAppMan", cAppMan_run);

#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C" void host_appman_run(void* ctx, int argc, char** argv);

extern "C" void cAppMan_run(void* ctx, int argc, char** argv) {
    host_appman_run(ctx, argc, argv);
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif
