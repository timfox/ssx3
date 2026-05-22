#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

// Retail @ 0x00345700 — intrusive list prepend (head at *list_head).
INCLUDE_ASM("mem/rom21_cEffectLink", cEffectLink_add);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void cEffectLink_add(void** list_head, void* node) {
    void* prev = *list_head;
    if (prev != 0) {
        *(void**)((char*)prev + 4) = node;
    }
    *(void**)((char*)node + 4) = list_head;
    void* prev_head = *list_head;
    *(void**)((char*)node + 0) = prev_head;
    *list_head = node;
}
#endif

#ifdef SSX3_HOST
}
#endif
