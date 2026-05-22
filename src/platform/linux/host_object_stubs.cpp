#include "platform/host_heap_synthetic.h"

#include <cstring>
#include <cstdint>

static bool host_ptr_looks_allocated(void* ptr) {
    if (ptr == 0) {
        return false;
    }
    const uintptr_t raw = reinterpret_cast<uintptr_t>(ptr);
    return raw >= 0x10000u && raw < 0x80000000u;
}

static int host_lowest_set_bit(unsigned masked) {
    for (int bit = 0; bit < 24; bit++) {
        if ((masked >> bit) & 1u) {
            return bit;
        }
    }
    return -1;
}

#ifdef SSX3_HOST
extern "C" {
#endif

static char g_host_bucket_man;

void* D_2898 = &g_host_bucket_man;

int D_004906F0;
int D_00491E00;
int D_00491340;
int D_00491200;

void* func_002D1BE0(void* block) {
    return block;
}

void func_00328F28(void* out_three_words, void* param_30) {
    (void)param_30;
    if (out_three_words) {
        int* out = (int*)out_three_words;
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
    }
}

void func_00328C20(void* node, int mode, void* hash_root, void* stack_indices) {
    (void)node;
    (void)mode;
    (void)hash_root;
    (void)stack_indices;
}

char D_0049E8D0[0x10] = {};
char D_0049E8E0[0x10] = {};
char D_0049E8F0[0x10] = {};
char D_0049E908[0x10] = {};
char D_0049E918[0x10] = {};
char D_0049E928[0x10] = {};
char D_0049E940[0x10] = {};
char D_0049E958[0x10] = {};
char D_0049E968[0x10] = {};
char D_0049E9D8[0x10] = {};
char D_0049E9E8[0x10] = {};

void func_0035F6E8(void* spline, void* config) {
    (void)spline;
    (void)config;
}

void func_0035A118(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_00357090(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_00356F68(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_0035DF70(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_00357358(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_00357798(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_0035F788(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_0035F1A8(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_00342808(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_003459A8(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_00345D80(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_003461C0(void* self, void* param) {
    (void)self;
    (void)param;
}

void func_0035BA88(void* self, void* param) {
    (void)self;
    (void)param;
}

int D_0049F250;
int D_0049F120;

int D_00517644 = 0;
int D_00517668 = 0xFFFFFFFF;

char D_0049E860[0x10] = {};
char D_0049E978[0x10] = {};
char D_0049E9A8[0x10] = {};
char D_00490AF0[0x10] = {};
char D_00490AD0[0x10] = {};
char D_0049E778[0x10] = {};
char D_0049E788[0x10] = {};
char D_0049E798[0x10] = {};
char D_0049E7A8[0x10] = {};
char D_0049E7B8[0x10] = {};

void func_003291E0(void* a, void* b) {
    (void)a;
    (void)b;
}

void func_003C5F5C(void* out, void* a, void* b) {
    (void)a;
    (void)b;
    if (out != 0 && b != 0) {
        std::memcpy(out, b, 0x40);
    }
}

void func_003458C0(void* particle, void* param) {
    (void)particle;
    (void)param;
}

void func_00346120(void* halo, void* param) {
    (void)halo;
    (void)param;
}

void func_003C5F94(void* fmt, void* a0) {
    (void)fmt;
    (void)a0;
}

void func_003C5F8C(void* fmt, unsigned long a0, unsigned long a1, unsigned long a2) {
    (void)fmt;
    (void)a0;
    (void)a1;
    (void)a2;
}

void func_0034FED8(void* a0, void* a1) {
    (void)a0;
    (void)a1;
}

void func_002F7A68(void* sub) {
    if (sub != 0) {
        std::memset(sub, 0, 0x1C0);
    }
}

void func_00416210(void* dst, void* a1, int size) {
    (void)a1;
    if (dst != 0 && size > 0) {
        std::memset(dst, 0, static_cast<size_t>(size));
    }
}

void func_003D415C(void* zero, void* scratch, void* overlap_out) {
    (void)zero;
    if (scratch != 0) {
        std::memset(scratch, 0, 0x30);
    }
    if (overlap_out != 0) {
        std::memset(overlap_out, 0, 0x10);
    }
}

void func_003D4AF0(void* block, void* scratch) {
    (void)block;
    (void)scratch;
}

void func_003D4B0E(void* block, void* overlap_out) {
    (void)block;
    if (overlap_out != 0) {
        std::memset(overlap_out, 0, 0x10);
    }
}

void func_003513D0(void* table) {
    if (table == 0) {
        return;
    }
    std::memset(table, 0, 0x40);
    std::memset((char*)table + 0x44, 0, 0x40);
    *(int*)((char*)table + 0x40) = 0;
}

void func_00351398(void* cache, void* entry) {
    if (cache == 0 || entry == 0) {
        return;
    }
    *(float*)((char*)cache + 0x44) = *(float*)((char*)entry + 0);
    *(float*)((char*)cache + 0x48) = *(float*)((char*)entry + 4);
    *(float*)((char*)cache + 0x4C) = *(float*)((char*)entry + 8);
    *(float*)((char*)cache + 0x50) = *(float*)((char*)entry + 12);
    *(float*)((char*)cache + 0x54) = *(float*)((char*)entry + 16);
    *(float*)((char*)cache + 0x58) = *(float*)((char*)entry + 20);
}

void func_003612C0(void* dst, void* src) {
    if (dst == 0 || src == 0) {
        return;
    }
    std::memcpy(dst, src, 0x40);
}

int D_0045288C;
int D_00452888;

// Retail @ 0x003D5290 — walk heap tag table @ ~0x517610 (4 rows, 0x928 stride).
void* func_003D5290(void* query_ptr) {
    if (query_ptr == 0) {
        return 0;
    }

#ifdef SSX3_HOST
    void* synthetic = host_heap_synthetic_resolve(query_ptr);
    if (synthetic != 0) {
        return synthetic;
    }
#endif

    if (host_ptr_looks_allocated(query_ptr)) {
        return query_ptr;
    }

    const uintptr_t raw = reinterpret_cast<uintptr_t>(query_ptr);
    const unsigned masked = (unsigned)(raw & 0x00ffffffu);
    if (host_lowest_set_bit(masked) < 0) {
        return 0;
    }

    return 0;
}

// Retail @ 0x003D3030 — func_003D5290 then load word at resolved+0x54 (or -8 if null).
int func_003D3030(void* block) {
    void* resolved = func_003D5290(block);
    if (resolved == 0) {
        return -8;
    }
    return *(int*)((char*)resolved + 0x54);
}

// Retail @ 0x003D11B8 — init/teardown scratch record (a1); status at out+0 for func_003D53B8.
void func_003D11B8(void* block, void* out) {
    if (out == 0) {
        return;
    }
    std::memset(out, 0, 0x40);
    void* resolved = func_003D5290(block);
    const int status = func_003D3030(block);
    *(int*)((char*)out + 0x24) = status;
    *(int*)((char*)out + 0x28) = (int)(unsigned long)resolved;
    *(int*)((char*)out + 0) = (status >= 0) ? 0 : -1;
    *(int*)((char*)out + 4) = -1;
    *(int*)((char*)out + 8) = -1;
}

#ifdef SSX3_HOST
}
#endif
