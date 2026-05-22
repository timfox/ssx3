#include <cstring>

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

void func_00351724(void* particle, void* param, void* heap, void* tag) {
    (void)particle;
    (void)param;
    (void)heap;
    (void)tag;
}

void func_003D415C(void* zero, void* scratch, void* overlap_out) {
    (void)zero;
    (void)scratch;
    (void)overlap_out;
}

void func_003D4AF0(void* block, void* scratch) {
    (void)block;
    (void)scratch;
}

void func_003D4B0E(void* block, void* overlap_out) {
    (void)block;
    (void)overlap_out;
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

#ifdef SSX3_HOST
}
#endif
