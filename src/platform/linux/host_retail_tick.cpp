#include "platform/host_retail.h"

#include "platform/host_pad.h"

#include <SDL.h>

extern "C" {

void SYNCTASK_run(void);
void THREAD_yieldticks(void);

void cExecutionMan_checkHalt(void* ctx) {
    (void)ctx;
}

void cExecutionMan_prepause(void* ctx) { (void)ctx; }

void cExecutionMan_postpause(void* ctx) { (void)ctx; }

extern int g_crowd_frame_tick;

void host_retail_tick(void* ssx_app) {
    (void)ssx_app;
    ++g_crowd_frame_tick;
    cInputPad_poll(0);
    SYNCTASK_run();
    THREAD_yieldticks();
}

} // extern "C"
