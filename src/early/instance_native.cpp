#include "common.h"

#include <cstdint>
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

#ifdef SSX3_HOST
static unsigned char g_replay_frame_root[0x200];
#endif

// Retail @ 0x002D1BD8 — load replay root pointer from GP-relative slot.
INCLUDE_ASM("early/early_func_002D1BD8", func_002D1BD8);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void* func_002D1BD8(void) {
#ifdef SSX3_HOST
    std::memset(g_replay_frame_root, 0, sizeof(g_replay_frame_root));
    return g_replay_frame_root;
#else
    return nullptr;
#endif
}
#endif

// Retail @ 0x001032C0 — instance flags + catalog index bump.
INCLUDE_ASM("early/early_func_001032C0", func_001032C0);
#ifdef SKIP_ASM
void func_001032C0(void* self, void* param_2, void* out_slot) {
    (void)param_2;
    if (self == 0) {
        return;
    }
    int flags = *(int*)((char*)self + 8);
    flags |= 0x100;
    int field_d4 = *(int*)((char*)self + 0xD4);
    if (field_d4 < 0) {
        field_d4 |= 0x300;
        *(int*)((char*)self + 0xD4) = field_d4;
    }
    flags &= ~0x200;
    *(int*)((char*)self + 8) = flags;

    int index = *(int*)((char*)self + 0xD8) + 1;
    *(int*)((char*)self + 0xD8) = index;
    if (out_slot != 0) {
        *(int*)((char*)out_slot + 0xDC) = index;
    }
}
#endif

#ifdef SSX3_HOST
static unsigned char g_D_004FE770[0x10000];
void* D_004FE770 = g_D_004FE770;
static void* g_instance_config_root;
void** D_002AF848 = &g_instance_config_root;
#else
extern char D_004FE770[];
extern void** D_002AF848;
#endif

// Retail @ 0x002D1BF0 — fetch instance config via global object interface.
INCLUDE_ASM("early/early_func_002D1BF0", func_002D1BF0);
#ifdef SKIP_ASM
void func_002D1BF0(void* self, void* param_2, void* config) {
    (void)param_2;
    (void)config;
    void* root = 0;
    if (D_002AF848 != 0) {
        root = *D_002AF848;
    }
    if (root == 0) {
        return;
    }
    void* iface = *(void**)((char*)root + 0x84);
    void* out_slot = *(void**)((char*)self + 0xA4);
    func_001032C0(self, iface, out_slot);
}
#endif

// Retail @ 0x002D9C00 — copy instance mesh matrices from catalog table.
INCLUDE_ASM("early/early_func_002D9C00", func_002D9C00);
#ifdef SKIP_ASM
static void instance_copy_matrix_block(void* self, const void* src_matrix) {
    if (self == 0 || src_matrix == 0) {
        return;
    }
    std::memcpy((char*)self + 0x60, src_matrix, 0x40);
    std::memcpy((char*)self + 0xB0, src_matrix, 0x40);
}

void func_002D9C00(void* self, void* param_2, void* config) {
    (void)param_2;
    (void)config;
    if (self == 0 || D_004FE770 == 0) {
        return;
    }

    char* cursor = (char*)D_004FE770;
    const char* end = (char*)D_004FE770 + 0xBC00;
    while (cursor < end) {
        void* entry = *(void**)(cursor + 0x2E0);
        if (entry == 0) {
            cursor += 0x2F0;
            continue;
        }

        const void* matrix = *(const void**)((char*)entry + 0xF8);
        if (matrix == 0) {
            cursor += 0x2F0;
            continue;
        }

        instance_copy_matrix_block(self, matrix);
        cursor += 0x2F0;
    }
}
#endif

// Retail @ 0x002D1CF0 — thin tail; calls func_002D9C00.
INCLUDE_ASM("early/early_func_002D1CF0", func_002D1CF0);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void func_002D1CF0(void* self, void* param_2, void* config) {
    func_002D9C00(self, param_2, config);
}
#endif

#ifdef SSX3_HOST
}
#endif
