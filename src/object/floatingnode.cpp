#include "common.h"

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
