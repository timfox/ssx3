#include "common.h"
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

#ifdef SSX3_HOST
static void cAirPredictor_beginLaunch(void* self) {
    (void)self;
}
static void cAirPredictor_finishLaunchSetup(void* self, void* a, void* b) {
    (void)self;
    (void)a;
    (void)b;
}
#else
extern void cAirPredictor_beginLaunch(void* self);
extern void cAirPredictor_finishLaunchSetup(void* self, void* a, void* b);
#endif

// Retail @ 0x00114198
INCLUDE_ASM("early/early_cAirPredictor", cAirPredictor_reset);
#ifdef SKIP_ASM
void cAirPredictor_reset(void* self) {
    *(int*)((char*)self + 0xAC) = 0;
    *(int*)((char*)self + 0xA0) = 0x50F16000;
    *(int*)((char*)self + 0x98) = 0;
    *(int*)((char*)self + 0x9C) = 0x50F12000;
    *(int*)((char*)self + 0xA4) = 0;
    *(short*)((char*)self + 0x30) = (short)-1;
    *(short*)((char*)self + 0x34) = (short)-1;
    *(short*)((char*)self + 0x3B) = (short)-1;
    *(int*)((char*)self + 0x90) = 0;
}
#endif

// Retail @ 0x001145B8
INCLUDE_ASM("early/early_cAirPredictor", cAirPredictor_startLaunchIntoAir);
#ifdef SKIP_ASM
void cAirPredictor_startLaunchIntoAir(void* self, void* param_2, void* param_3) {
    (void)param_3;
    cAirPredictor_beginLaunch(self);
    cAirPredictor_finishLaunchSetup(self, param_2, param_3);
}
#endif

// Retail @ 0x00114618
INCLUDE_ASM("early/early_cAirPredictor", cAirPredictor_initLaunch);
#ifdef SKIP_ASM
void cAirPredictor_initLaunch(void* self) {
    *(int*)((char*)self + 0xAC) = 0;
    *(float*)((char*)self + 0xA0) = 0.0f;
    *(float*)((char*)self + 0x98) = 0.0f;
}
#endif

#ifdef SSX3_HOST
}
#endif
