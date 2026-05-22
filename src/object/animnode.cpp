#include "common.h"
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void func_003C5F8C(void* fmt, unsigned long a0, unsigned long a1, unsigned long a2);
extern void func_003513D0(void* table);
extern void func_00351398(void* cache, void* entry);
extern void func_003612C0(void* dst, void* src);

extern char D_0049E860[];
extern char D_00490AF0[];
extern char D_00490AD0[];

// Retail @ 0x0034F9A0 — mesh cache rebuild (large; ObjDiff unit).
INCLUDE_ASM("mem/rom21_cAnimNode", cAnimNode_setAnimMeshCache);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void cAnimNode_setAnimMeshCache(void* self) {
    if (self == 0) {
        return;
    }

    void* link = *(void**)((char*)self + 0x2C);
    if (link == 0) {
        return;
    }

    void* mesh = *(void**)((char*)link + 0x80);
    if (mesh == 0) {
        return;
    }

    const int count = *(int*)((char*)self + 4);
    if (count < 0) {
        return;
    }

    *(int*)((char*)self + 0x40) = 0;

    int active = 0;
    char* entry_walk = (char*)mesh + 8;
    for (int i = 0; i < count; i++) {
        if (*(void**)(entry_walk + 8) != 0) {
            active += 1;
        }
        entry_walk += 0x10;
    }
    *(int*)((char*)self + 0x40) = active;

    void* cache = nullptr;
    if (active == 0) {
        cache = cMemMan_alloc(0xD0UL, (void*)D_0049E860, 0x20000000UL, (void*)0);
        if (cache == 0) {
            func_003C5F8C((void*)D_0049E860, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0UL);
            return;
        }
        *(void**)((char*)self + 0x40) = cache;

        *(void**)((char*)self + 0x84) = (void*)D_00490AF0;
        *(void**)((char*)self + 0x88) = (void*)D_00490AD0;
        func_003513D0((char*)cache + 0x48);
        func_003612C0((char*)cache + 0x4C, (char*)cache + 0x4C);
    }

    cache = *(void**)((char*)self + 0x40);
    if (cache == 0) {
        return;
    }

    entry_walk = (char*)mesh + 8;
    for (int i = 0; i < count; i++) {
        void* entry = *(void**)(entry_walk + 8);
        if (entry != 0) {
            func_00351398(cache, entry);
        }
        entry_walk += 0x10;
    }

    *(void**)mesh = cache;
}
#endif

#ifdef SSX3_HOST
}
#endif
