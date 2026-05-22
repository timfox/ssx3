#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
#include <cstring>
extern "C" {
#endif

extern unsigned char D_004FF120[0x50];
extern unsigned char D_004FF1A0[0x50];
extern unsigned int AIrand();

static void basis_scale_vec4(float* dst, const float* src, float scale) {
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i] * scale;
    }
}

// Retail @ 0x0036CE00 — basis+0x10/0x18/0x20 from param floats.
void func_0036CE00(void* basis, float f12, float f13, float f14) {
    if (basis == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    const float kOnePointFive = 1.5f;
    *(float*)(b + 0x10) = f14;
    const float t = f14 * kOnePointFive;
    *(float*)(b + 0x20) = f13 - f12;
    *(float*)(b + 0x18) = f12 - t;
}

// Retail @ 0x0036CE28 — basis+0x14/0x1C using basis+0x4C scale.
void func_0036CE28(void* basis, float f12, float f13) {
    if (basis == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    const float scale = *(float*)(b + 0x4C);
    f13 = f13 - f12;
    f12 = f12 * scale;
    f13 = f13 * scale;
    f12 = f12 - f13;
    *(float*)(b + 0x14) = f13;
    *(float*)(b + 0x1C) = f12;
}

// Retail @ 0x0036CBF8 — particle+0x20 basis block setup (scalar + template copy).
INCLUDE_ASM("mem/rom21_func_0036CBF8", func_0036CBF8);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(func_0036CBF8, ".word 0x44850000\n\t")
#else
void func_0036CBF8(void* out_basis, void* zero_word, void* param_at_4, void* stack) {
    if (out_basis == 0 || stack == 0) {
        return;
    }
    char* out = static_cast<char*>(out_basis);
    char* stk = static_cast<char*>(stack);
    char* param = static_cast<char*>(param_at_4);

    const int zero_i = zero_word != 0 ? *static_cast<int*>(zero_word) : 0;
    float f0 = static_cast<float>(zero_i);
    if (f0 == 0.0f) {
        f0 = 1.0f;
    }

    const int word4 = param != 0 ? *reinterpret_cast<int*>(param) : 0;
    *(int*)(out + 0x00) = zero_i;
    *(int*)(out + 0x04) = word4;

    float f12 = 0.0f;
    float f13 = 0.0f;
    float f14 = 0.0f;
    if (param != 0) {
        char* param_base = param - 4;
        f13 = *reinterpret_cast<float*>(param_base + 0x20);
        f14 = *reinterpret_cast<float*>(param_base + 0x0C);
        const float p14 = *reinterpret_cast<float*>(param_base + 0x14);
        const float p1c = *reinterpret_cast<float*>(param_base + 0x1C);
        f12 = p14 + p1c * 0.5f;
    }

    f12 *= f14;
    f13 *= f14;
    f12 /= f0;
    *reinterpret_cast<float*>(out + 0x0C) = f12;

    const float q = (f14 != 0.0f) ? (1.0f / f14) : 1.0f;
    basis_scale_vec4(reinterpret_cast<float*>(stk + 0x00),
                     reinterpret_cast<float*>(stk + 0x00), q);
    basis_scale_vec4(reinterpret_cast<float*>(stk + 0x10),
                     reinterpret_cast<float*>(stk + 0x10), q);

    float word4f = static_cast<float>(word4);
    if (word4f == 0.0f) {
        word4f = 1.0f;
    }
    const float inv = 1.0f / word4f;
    *reinterpret_cast<float*>(out + 0x30) = inv;
    *reinterpret_cast<float*>(out + 0x34) = f13;
    *reinterpret_cast<float*>(out + 0x4C) = f14;

    std::memcpy(out + 0x60, stk + 0x00, 16);
    std::memcpy(out + 0x140, stk + 0x00, 16);
    std::memcpy(out + 0x130, D_004FF120, 16);
}
#endif
#endif

// Retail @ 0x00370058 — particle basis finalize (GTE-heavy; host covers prologue).
INCLUDE_ASM("mem/rom21_func_00370058", func_00370058);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(func_00370058, ".word 0x27bdfff0\n\t")
#else
void func_00370058(void* basis, void* tmpl, void* param, void* particle_sub) {
    (void)particle_sub;
    if (basis == 0 || param == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    char* prm = static_cast<char*>(param);

    const float f12 = *reinterpret_cast<float*>(prm + 0x10);
    const float f13 = *reinterpret_cast<float*>(prm + 0xCC);
    const float f14 = *reinterpret_cast<float*>(prm + 0x18);
    func_0036CE00(basis, f12, f13, f14);

    const float f12b = *reinterpret_cast<float*>(prm + 0x1C);
    const float f13b = *reinterpret_cast<float*>(prm + 0x14);
    func_0036CE28(basis, f12b, f13b);

    if (tmpl != 0) {
        std::memcpy(b + 0xB0, tmpl, 16);
        std::memcpy(b + 0x00, tmpl, 16);
    }

    const float p24 = *reinterpret_cast<float*>(prm + 0x24);
    if (p24 < 0.0f) {
        *reinterpret_cast<float*>(b + 0x38) = *reinterpret_cast<float*>(prm + 0x38);
    }
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif
