#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

#ifdef SSX3_HOST
static void cRider_initOnce_precheck(void* self) { (void)self; }
static void cRider_initSub880(void* self, int flag) {
    (void)self;
    (void)flag;
}
#else
extern void cRider_initOnce_precheck(void* self);
extern void cRider_initSub880(void* self, int flag);
#endif

// Retail @ 0x0011C230 (yaml 0x01D230)
INCLUDE_ASM("early/early_cRider_initOnce", cRider_initOnce);
#ifdef SKIP_ASM
void cRider_initOnce(void* self) {
    if (!self) {
        return;
    }
    cRider_initOnce_precheck(self);
    for (int i = 0; i < 7; i++) {
        cRider_initSub880(self, 1);
    }
}
#endif

#ifdef SSX3_HOST
}
#endif
