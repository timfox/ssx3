#include "common.h"
#include <cmath>
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void func_003C5F8C(void* fmt, unsigned long a0, unsigned long a1, unsigned long a2);
extern void* func_002D1BE0(void* block);
extern void func_003291E0(void* a, void* b);

extern char D_0049E778[];
extern char D_0049E788[];
extern char D_0049E798[];
extern char D_0049E7A8[];
extern char D_0049E7B8[];

static void flexbridge_run_config_init(void* self, void* config) {
    (void)config;
    void* cfg = *(void**)((char*)self + 0x50);
    if (cfg == 0) {
        return;
    }
    char* iface = *(char**)((char*)cfg + 0x10D8);
    if (iface == 0) {
        return;
    }
    const short src_offset = *(short*)((char*)iface + 0x260);
    typedef void (*FlexBridgeConfigFn)(void*, void*);
    FlexBridgeConfigFn init = *(FlexBridgeConfigFn*)((char*)iface + 0x264);
    if (init != 0) {
        init((char*)cfg + src_offset, self);
    }
}

static void* flexbridge_alloc_or_fail(unsigned long size, void* heap_tag) {
    void* mem = cMemMan_alloc(size, heap_tag, 0x20000000UL, (void*)0);
    if (mem == 0) {
        func_003C5F8C(heap_tag, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0UL);
    }
    return mem;
}

static void flexbridge_fill_grid_points(void* self, void* grid, int point_count) {
    if (self == 0 || grid == 0 || point_count < 2) {
        return;
    }
    float extent = *(float*)((char*)self + 0x18);
    if (extent <= 0.0f) {
        extent = 1.0f;
    }
    for (int i = 0; i < point_count; i++) {
        const float t = static_cast<float>(i) / static_cast<float>(point_count - 1);
        char* point = static_cast<char*>(grid) + (i * 0x10);
        *(float*)(point + 0) = (t - 0.5f) * extent * 2.0f;
        *(float*)(point + 4) = 0.0f;
        *(float*)(point + 8) = t * extent;
    }
}

// Retail @ 0x00348778 — flex bridge grid setup (large; ObjDiff unit).
INCLUDE_ASM("mem/rom21_cFlexBridgeNode", cFlexBridgeNode_setupGrid);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void cFlexBridgeNode_setupGrid(void* self, void* config) {
    flexbridge_run_config_init(self, config);

    void* primary = flexbridge_alloc_or_fail(0x50UL, (void*)D_0049E778);
    void* secondary = flexbridge_alloc_or_fail(0x90UL, (void*)D_0049E788);
    void* tertiary = flexbridge_alloc_or_fail(0x100UL, (void*)D_0049E798);
    void* quaternary = flexbridge_alloc_or_fail(0x100UL, (void*)D_0049E7A8);

    *(void**)((char*)self + 0) = primary;
    *(void**)((char*)self + 0x80) = secondary;
    *(int*)((char*)self + 0x78) = 0;
    *(int*)((char*)self + 0x84) = 3;

    if (primary != 0) {
        std::memset(primary, 0, 0x50);
        (void)func_002D1BE0(primary);
    }
    if (secondary != 0) {
        std::memset(secondary, 0, 0x90);
    }
    if (tertiary != 0) {
        std::memset(tertiary, 0, 0x100);
    }
    if (quaternary != 0) {
        std::memset(quaternary, 0, 0x100);
    }
    (void)D_0049E7B8;

    constexpr int kGridPoints = 7;
    flexbridge_fill_grid_points(self, secondary, kGridPoints);
    *(int*)((char*)self + 0x7C) = kGridPoints;

    for (int i = 0; i < kGridPoints; i++) {
        *(short*)((char*)self + 0x84 + (i * 2)) = static_cast<short>(i);
    }

    if (tertiary != 0 && quaternary != 0) {
        func_003291E0(tertiary, quaternary);
    }
}
#endif

#ifdef SSX3_HOST
}
#endif
