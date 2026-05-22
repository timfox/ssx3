#include "common.h"
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

void func_0034FB00(void* self, void* param_2, void* config);
void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void func_003C5F8C(void* fmt, unsigned long a0, unsigned long a1, unsigned long a2);
#ifdef SSX3_HOST
static unsigned char g_D_00491980[8];
static unsigned char g_D_0049E8A8[8];
void* D_00491980 = g_D_00491980;
void* D_0049E8A8 = g_D_0049E8A8;
#else
extern char D_00491980[];
extern char D_0049E8A8[];
#endif

static void deadfade_store_basis_matrix(float* matrix, float x, float y, float z) {
    std::memset(matrix, 0, 16 * sizeof(float));
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;
    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
}

static void deadfade_build_transform_ring(void* self, void* block) {
    int count = *(int*)((char*)self + 4);
    if (count <= 0 || count > 16) {
        return;
    }
    char* table = *(char**)((char*)self + 0x38);
    if (table == 0 || block == 0) {
        return;
    }
    for (int i = 0; i < count; i++) {
        char* entry = table + (i * 0x10);
        float x = *(float*)(entry + 0);
        float y = *(float*)(entry + 4);
        float z = *(float*)(entry + 8);
        float matrix[16];
        deadfade_store_basis_matrix(matrix, x, y, z);
        std::memcpy((char*)block + (i * 0x40), matrix, 0x40);
    }
}

static void deadfade_init_fade_table(void* self, void* block) {
    int count = *(int*)((char*)self + 4);
    if (count == -1) {
        return;
    }
    char* table = *(char**)((char*)self + 0x38);
    if (table == 0 || block == 0) {
        return;
    }
    const float extent = *(float*)((char*)self + 0x18);
    const float scale = extent > 0.0f ? extent : 1.0f;
    for (int i = 0; i < 16; i++) {
        if (i >= count) {
            break;
        }
        char* entry = table + (i * 0x10);
        const int corner = i & 7;
        const float x = (corner & 1) ? scale : -scale;
        const float y = (corner & 2) ? scale : -scale;
        const float z = (corner & 4) ? scale : -scale;
        *(float*)(entry + 0) = x;
        *(float*)(entry + 4) = y;
        *(float*)(entry + 8) = z;
    }
    (void)block;
}

// Retail @ 0x00350750 — instance node + fade state (large; ObjDiff unit).
INCLUDE_ASM("mem/rom21_cDeadFadeNode", cDeadFadeNode_cDeadFadeNode);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void* cDeadFadeNode_cDeadFadeNode(void* self, void* param_2, void* config) {
    func_0034FB00(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)D_00491980;
    *(int*)((char*)self + 0x1C) = -1;
    *(unsigned char*)((char*)self + 0x23) = 7;
    *(unsigned char*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(float*)((char*)self + 8) = 1.0f;
    *(float*)((char*)self + 0x28) = 1.0f;
    *(float*)((char*)self + 0x30) = 1.0f;

    void* block = cMemMan_alloc(0x180UL, D_0049E8A8, 0x20000000UL, (void*)0);
    if (block == 0) {
        func_003C5F8C((void*)D_0049E8A8, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0UL);
        return self;
    }
    *(void**)((char*)self + 0) = block;

    char* iface = *(char**)config;
    if (iface != 0) {
        void (*init_primary)(void*, void*) =
            *(void (**)(void*, void*))((char*)iface + 0xD0);
        if (init_primary != 0) {
            init_primary(block, self);
        }

        void (*init_secondary)(void*, void*) =
            *(void (**)(void*, void*))((char*)iface + 0xD8);
        if (init_secondary != 0) {
            void* alt = cMemMan_alloc(0x180UL, D_0049E8A8, 0x20000000UL, (void*)0);
            if (alt != 0) {
                init_secondary(alt, self);
                char* src = *(char**)((char*)self + 0x38);
                if (src != 0) {
#ifdef SSX3_HOST
                    std::memcpy(block, src, 0x180);
#else
                    func_003E6574(block, src, 0x180);
#endif
                }
            }
        }
    }

    deadfade_init_fade_table(self, block);
    deadfade_build_transform_ring(self, block);

    int flags = *(int*)((char*)self + 8);
    flags = (flags & 0xFFFFFF00) | ((flags & 0xFF) + 4);
    *(int*)((char*)self + 8) = flags;

    return self;
}
#endif

#ifdef SSX3_HOST
}
#endif
