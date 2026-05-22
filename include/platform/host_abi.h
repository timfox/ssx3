#ifndef PLATFORM_HOST_ABI_H
#define PLATFORM_HOST_ABI_H

/* Host implementations of PS2 game symbols (C linkage for future decomp). */

#include "platform/host.h"

#ifdef __cplusplus
extern "C" {
#endif

void systemInit(void* a0, void* a1, int a2, void* a3);

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
void cMemMan_free(void* ptr);
void cMemMan_initialize(void* a0, void* a1, int a2, void* a3);

void cExecutionMan_halt(void* ctx);

void host_appman_run(void* ctx, int argc, char** argv);
void cAppMan_run(void* ctx, int argc, char** argv);
void host_appman_main_loop(void* ctx);
void cAppMan_mainLoop(void* ctx);

/* Host tick loop helpers (used by cAppMan_mainLoop stub). */
int host_app_tick(void);
void host_app_request_quit(void);
void host_app_reset_quit(void);
int host_app_tick_count(void);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_ABI_H */
