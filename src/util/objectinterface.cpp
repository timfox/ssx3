#include "common.h"

#ifdef SSX3_HOST
extern "C" {
extern void** D_002AF7B8;
}
#else
extern void** D_002AF7B8;
#endif

// Retail @ 0x002D1CE0 — global object interface → instance manager.
INCLUDE_ASM("early/early_cObjectInterface", cObjectInterface_getInstanceMan);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void* cObjectInterface_getInstanceMan(void) {
    void* iface = 0;
    if (D_002AF7B8 != 0) {
        iface = *D_002AF7B8;
    }
    if (iface == 0) {
        return 0;
    }
    return *(void**)((char*)iface + 0x88);
}
#endif
