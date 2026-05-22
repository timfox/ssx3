#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

// Retail @ 0x00345700 — intrusive list prepend (head at *list_head).
INCLUDE_ASM("mem/rom21_cEffectLink", cEffectLink_add);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cEffectLink_add,
    ".word 0x8c820000\n\t"
    ".word 0x54400001\n\t"
    ".word 0xac450004\n\t"
    ".word 0xaca40004\n\t"
    ".word 0x8c820000\n\t"
    ".word 0xaca20000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac850000\n\t"
)
#else
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
