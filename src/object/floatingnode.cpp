#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
static unsigned char g_D_0049E890[0x10];
void* D_0049E890 = g_D_0049E890;
}
#else
extern char D_0049E890[];
#endif

#ifdef SSX3_HOST
extern "C" {
#endif
void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
void cInstanceNode_getBoundBoxInfo(void* self, void* out, unsigned long flags);
#ifdef SSX3_HOST
}
#endif

// Retail @ 0x0034EEB8
INCLUDE_ASM("mem/rom21_cFloatingNode", cFloatingNode_initInfo);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cFloatingNode_initInfo,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24a5e890\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040030\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x0200202d\n\t"
    ".word 0xae020078\n\t"
    ".word 0x0c0d415c\n\t"
    ".word 0x24a60020\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#else
void cFloatingNode_initInfo_impl(void* self);
#ifdef SSX3_HOST
extern "C"
#endif
void cFloatingNode_initInfo(void* self) {
    cFloatingNode_initInfo_impl(self);
}

void cFloatingNode_initInfo_impl(void* self) {
    void* block = cMemMan_alloc(0x30UL, (void*)D_0049E890, 0x20000000UL, (void*)0);
    *(void**)((char*)self + 0x78) = block;
    cInstanceNode_getBoundBoxInfo(self, block, 0x20000000UL);
}
#endif
#endif

