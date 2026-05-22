#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

#ifdef SSX3_HOST
static void cAI_applyStateChange(void* self, int state) {
    (void)self;
    (void)state;
}
#else
extern void cAI_applyStateChange(void* self, int state);
#endif

// Retail @ 0x00114B10 (yaml 0x014B10)
INCLUDE_ASM("early/early_cAI", cAI_setAIState);
#ifdef SKIP_ASM
void cAI_setAIState(void* self, int state) {
    if (*(int*)self == state) {
        return;
    }
    int saved = *(int*)((char*)self + 0x98);
    *(int*)((char*)self + 4) = state;
    cAI_applyStateChange(self, state);
    *(int*)((char*)self + 0x9C) = saved;
}
#endif

#ifdef SSX3_HOST
}
#endif
