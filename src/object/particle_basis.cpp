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

/* EE vmulax/vmadday/vmaddaz/vmaddw: column-major 4x4 * vec4. */
static void particle_mat4_mul_vec4(float* out, const float* mat, float vx, float vy, float vz, float vw) {
    out[0] = mat[0] * vx + mat[4] * vy + mat[8] * vz + mat[12] * vw;
    out[1] = mat[1] * vx + mat[5] * vy + mat[9] * vz + mat[13] * vw;
    out[2] = mat[2] * vx + mat[6] * vy + mat[10] * vz + mat[14] * vw;
    out[3] = mat[3] * vx + mat[7] * vy + mat[11] * vz + mat[15] * vw;
}

static void particle_gte_vec_scale_uniform(float* v, float scale) {
    if (scale == 0.0f) {
        return;
    }
    const float q = 1.0f / scale;
    for (int i = 0; i < 4; i++) {
        v[i] *= q;
    }
}

static void particle_gte_store_quad(char* basis, int offset, float* v, int zero_w) {
    if (zero_w) {
        v[3] = 0.0f;
    }
    std::memcpy(basis + offset, v, 16);
}

static float particle_basis_rand01(void) {
    unsigned int bits = AIrand() & 0x7FFFFFu;
    bits |= 0x3F800000u;
    float unit;
    std::memcpy(&unit, &bits, sizeof(unit));
    return unit - 1.0f;
}

static void particle_quad_add(float* dst, const float* a, const float* b) {
    for (int i = 0; i < 4; i++) {
        dst[i] = a[i] + b[i];
    }
}

static void particle_quad_sub(float* dst, const float* a, const float* b) {
    for (int i = 0; i < 4; i++) {
        dst[i] = a[i] - b[i];
    }
}

static void particle_quad_scale(float* v, float scale) {
    for (int i = 0; i < 4; i++) {
        v[i] *= scale;
    }
}

static void particle_quad_scale_copy(float* dst, const float* src, float scale) {
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i] * scale;
    }
}

/* gp-0x2868/0x2864/0x2860 with gp=0x0049E9D8 → D_0049C170..0x178 in lit4.s */
static const float kParticleD500_GpScaleCap = 0.465207994f;
static const float kParticleD500_GpScaleMul = 0.1779260039f;
static const float kParticleD500_GpScaleAdd = 0.4411740005f;

/* bc1tl: if (candidate < current) current = candidate */
static void particle_sub_clamp_min(void* sub, int off, float candidate) {
    if (sub == 0) {
        return;
    }
    float* slot = reinterpret_cast<float*>(static_cast<char*>(sub) + off);
    if (candidate < *slot) {
        *slot = candidate;
    }
}

/* bc1tl with c.lt.s current, candidate: if (current < candidate) current = candidate */
static void particle_sub_clamp_max(void* sub, int off, float candidate) {
    if (sub == 0) {
        return;
    }
    float* slot = reinterpret_cast<float*>(static_cast<char*>(sub) + off);
    if (*slot < candidate) {
        *slot = candidate;
    }
}

static void particle_sub_clamp_vec3_min_pair(void* sub, const float* candidate) {
    if (sub == 0 || candidate == 0) {
        return;
    }
    particle_sub_clamp_min(sub, 0x00, candidate[0]);
    particle_sub_clamp_min(sub, 0x04, candidate[1]);
    particle_sub_clamp_min(sub, 0x08, candidate[2]);
    particle_sub_clamp_min(sub, 0x10, candidate[0]);
    particle_sub_clamp_min(sub, 0x14, candidate[1]);
    particle_sub_clamp_min(sub, 0x18, candidate[2]);
}

static void particle_sub_clamp_vec3_max_pair(void* sub, const float* candidate) {
    if (sub == 0 || candidate == 0) {
        return;
    }
    particle_sub_clamp_max(sub, 0x00, candidate[0]);
    particle_sub_clamp_max(sub, 0x04, candidate[1]);
    particle_sub_clamp_max(sub, 0x08, candidate[2]);
    particle_sub_clamp_max(sub, 0x10, candidate[0]);
    particle_sub_clamp_max(sub, 0x14, candidate[1]);
    particle_sub_clamp_max(sub, 0x18, candidate[2]);
}

/* D500 keeps a 0xB0-byte stack frame; only these slots are needed for host parity. */
struct particle_d500_work {
    alignas(16) char bytes[0xB0];
};

static float* particle_d500_slot(particle_d500_work* w, int byte_off) {
    return reinterpret_cast<float*>(w->bytes + byte_off);
}

static void particle_work_clamp_min(particle_d500_work* w, int dst, int src) {
    float* d = particle_d500_slot(w, dst);
    const float* s = particle_d500_slot(w, src);
    for (int i = 0; i < 3; i++) {
        if (s[i] < d[i]) {
            d[i] = s[i];
        }
    }
}

static void particle_work_clamp_max(particle_d500_work* w, int dst, int src) {
    float* d = particle_d500_slot(w, dst);
    const float* s = particle_d500_slot(w, src);
    for (int i = 0; i < 3; i++) {
        if (d[i] < s[i]) {
            d[i] = s[i];
        }
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

// Retail @ 0x0036CCB8 — BXrand fills basis+0x38..0x5C (PS2: src/mem/units/rom21_func_0036CCB8.c).
#ifdef SSX3_HOST
void func_0036CCB8(void* basis) {
    if (basis == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    const int offs[] = {0x38, 0x3C, 0x40, 0x44, 0x48, 0x50, 0x54, 0x58, 0x5C};
    for (int off : offs) {
        *reinterpret_cast<float*>(b + off) = particle_basis_rand01();
    }
}
#endif

// Retail @ 0x0036D008 — copy template quad to basis+0x60 (PS2 unit).
#ifdef SSX3_HOST
void func_0036D008(void* basis) {
    if (basis == 0) {
        return;
    }
    std::memcpy(static_cast<char*>(basis) + 0x60, D_004FF1A0, 16);
}
#endif

// Retail @ 0x0036D318 — stack quads + $f12 → basis+0xF0/0x100/0x110/0x120 (PS2 unit).
#ifdef SSX3_HOST
static void particle_basis_d318(void* basis,
                                void* sp_base,
                                void* sp_20,
                                void* sp_40,
                                void* sp_60,
                                float f12) {
    if (basis == 0 || sp_base == 0 || sp_20 == 0 || sp_40 == 0 || sp_60 == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    const float* sp0 = static_cast<const float*>(sp_base);
    const float* sp20 = static_cast<const float*>(sp_20);
    const float* sp40 = static_cast<const float*>(sp_40);
    const float* sp60 = static_cast<const float*>(sp_60);

    const float kOnePointFive = 1.5f;
    float sum[4];
    float diff[4];
    float tmp[4];

    particle_quad_add(sum, sp40, sp60);
    particle_quad_scale(sum, kOnePointFive);
    particle_quad_sub(diff, sp0, sum);
    std::memcpy(b + 0xF0, diff, 16);

    particle_quad_sub(tmp, sp20, sp0);
    const float scale4c = *reinterpret_cast<float*>(b + 0x4C);
    f12 *= scale4c;
    if (f12 != 0.0f) {
        particle_gte_vec_scale_uniform(tmp, f12);
    }
    std::memcpy(b + 0x120, tmp, 16);

    std::memcpy(b + 0x100, sp_40, 16);
    std::memcpy(b + 0x110, sp_60, 16);
}

void func_0036D318(void* basis,
                   void* sp_base,
                   void* sp_20,
                   void* sp_40,
                   void* sp_60,
                   float f12) {
    particle_basis_d318(basis, sp_base, sp_20, sp_40, sp_60, f12);
}
#endif

// Retail @ 0x0036D3D8 — basis+0x08 = $f12 + basis+0x4C (PS2 unit).
#ifdef SSX3_HOST
void func_0036D3D8(void* basis, float f12) {
    if (basis == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    *reinterpret_cast<float*>(b + 0x08) = *reinterpret_cast<float*>(b + 0x4C) + f12;
}
#endif

// Retail @ 0x0036D500 — particle_sub min/max from basis B0/C0/D0 (PS2: rom21_func_0036D500.c, ~11 KiB).
#ifdef SSX3_HOST
static float particle_d500_pick_scale(const char* basis) {
    const float f0 = *reinterpret_cast<const float*>(basis + 0x14);
    const float f1 = *reinterpret_cast<const float*>(basis + 0x1C);
    const float sum_a = f1 + f0;
    const float sum_b = f1 + f0 + f0;
    return (sum_b < sum_a) ? sum_b : sum_a;
}

static void particle_d500_clamp_sub_from_work60(void* particle_sub, const particle_d500_work* work) {
    const float* m = particle_d500_slot(const_cast<particle_d500_work*>(work), 0x60);
    particle_sub_clamp_min(particle_sub, 0x00, m[0]);
    particle_sub_clamp_min(particle_sub, 0x04, m[1]);
    particle_sub_clamp_min(particle_sub, 0x08, m[2]);
    particle_sub_clamp_max(particle_sub, 0x10, m[0]);
    particle_sub_clamp_max(particle_sub, 0x14, m[1]);
    particle_sub_clamp_max(particle_sub, 0x18, m[2]);
}

/* Retail 0x0036DB80: seed D500 stack from current particle_sub. */
static void particle_d500_seed_work(particle_d500_work* work, void* particle_sub) {
    std::memcpy(work->bytes + 0x00, particle_sub, 16);
    std::memcpy(work->bytes + 0x10, static_cast<char*>(particle_sub) + 0x10, 16);
}

/* Retail 0x0036DB90–0x0036DC8C: basis+0x70..0xA0 into work, scalar min/max on work slots. */
static void particle_d500_pass_basis70_a0(void* basis, particle_d500_work* work) {
    char* b = static_cast<char*>(basis);
    const float kTwo = 2.0f;
    float q[4];
    float t[4];

    particle_quad_add(q,
                      reinterpret_cast<float*>(b + 0x70),
                      reinterpret_cast<float*>(b + 0x80));
    std::memcpy(work->bytes + 0x60, q, 16);

    particle_quad_add(q, reinterpret_cast<float*>(work->bytes + 0x60),
                      reinterpret_cast<float*>(b + 0x90));
    std::memcpy(work->bytes + 0x50, q, 16);

    particle_quad_add(q, reinterpret_cast<float*>(work->bytes + 0x50),
                      reinterpret_cast<float*>(b + 0xA0));
    std::memcpy(work->bytes + 0x20, q, 16);
    std::memcpy(work->bytes + 0x40, q, 16);
    std::memcpy(work->bytes + 0x30, q, 16);

    std::memcpy(t, b + 0x80, 16);
    particle_quad_scale(t, kTwo);
    std::memcpy(work->bytes + 0x80, t, 16);

    particle_quad_add(q, reinterpret_cast<float*>(b + 0x70),
                      reinterpret_cast<float*>(work->bytes + 0x80));
    std::memcpy(work->bytes + 0x70, q, 16);

    particle_quad_add(q, reinterpret_cast<float*>(work->bytes + 0x60),
                      reinterpret_cast<float*>(b + 0x90));
    std::memcpy(work->bytes + 0x80, q, 16);

    particle_quad_add(q, reinterpret_cast<float*>(work->bytes + 0x50),
                      reinterpret_cast<float*>(b + 0xA0));
    std::memcpy(work->bytes + 0x40, q, 16);

    particle_work_clamp_min(work, 0x20, 0x40);
    particle_work_clamp_min(work, 0x24, 0x44);
    particle_work_clamp_min(work, 0x28, 0x48);
    particle_work_clamp_max(work, 0x30, 0x40);
    particle_work_clamp_max(work, 0x34, 0x44);
    particle_work_clamp_max(work, 0x38, 0x48);
}

/* Retail 0x0036E0DC–0x0036E1EC: second 70/80/90/A0 chain (same GTE pattern). */
static void particle_d500_pass_basis70_a0_b(void* basis, particle_d500_work* work) {
    char* b = static_cast<char*>(basis);
    const float kTwo = 2.0f;
    float q[4];
    float t[4];

    particle_quad_add(q,
                      reinterpret_cast<float*>(b + 0x70),
                      reinterpret_cast<float*>(b + 0x80));
    std::memcpy(work->bytes + 0x60, q, 16);

    std::memcpy(t, b + 0x90, 16);
    particle_quad_scale(t, kTwo);
    std::memcpy(work->bytes + 0x70, t, 16);

    particle_quad_add(q, reinterpret_cast<float*>(work->bytes + 0x60),
                      reinterpret_cast<float*>(work->bytes + 0x70));
    std::memcpy(work->bytes + 0x50, q, 16);

    std::memcpy(t, b + 0xA0, 16);
    particle_quad_scale(t, kTwo);
    std::memcpy(work->bytes + 0x80, t, 16);

    particle_quad_add(q, reinterpret_cast<float*>(work->bytes + 0x50),
                      reinterpret_cast<float*>(work->bytes + 0x80));
    std::memcpy(work->bytes + 0x40, q, 16);

    particle_work_clamp_min(work, 0x20, 0x40);
    particle_work_clamp_min(work, 0x24, 0x44);
    particle_work_clamp_min(work, 0x28, 0x48);
    particle_work_clamp_max(work, 0x30, 0x40);
    particle_work_clamp_max(work, 0x34, 0x44);
    particle_work_clamp_max(work, 0x38, 0x48);
}

/* Retail 0x0036E318–0x0036E554: basis+0x60 vs work; gp-scaled GTE; push into particle_sub. */
static void particle_d500_apply_work_to_sub(void* basis,
                                            particle_d500_work* work,
                                            void* particle_sub,
                                            float f3_scale) {
    char* b = static_cast<char*>(basis);
    float q[4];
    float t[4];
    float u[4];

    float f1 = f3_scale;
    if (f1 > kParticleD500_GpScaleCap) {
        f1 = kParticleD500_GpScaleCap;
    }
    float f0 = f1 * kParticleD500_GpScaleMul;

    particle_quad_sub(q,
                      reinterpret_cast<float*>(b + 0x60),
                      reinterpret_cast<float*>(work->bytes + 0x30));
    std::memcpy(work->bytes + 0x50, q, 16);

    particle_quad_sub(q,
                      reinterpret_cast<float*>(b + 0x60),
                      reinterpret_cast<float*>(work->bytes + 0x20));
    std::memcpy(work->bytes + 0x60, q, 16);

    particle_quad_scale_copy(t, reinterpret_cast<float*>(b + 0x60), f0);
    std::memcpy(work->bytes + 0x90, t, 16);

    f0 = f0 + kParticleD500_GpScaleAdd;
    f1 = f1 * f0;

    particle_quad_add(q,
                      reinterpret_cast<float*>(work->bytes + 0x00),
                      reinterpret_cast<float*>(work->bytes + 0x80));
    std::memcpy(work->bytes + 0x90, q, 16);
    std::memcpy(work->bytes + 0x70, q, 16);

    particle_quad_scale_copy(t, reinterpret_cast<float*>(work->bytes + 0x50), f1);
    particle_quad_add(q,
                      reinterpret_cast<float*>(work->bytes + 0x70),
                      reinterpret_cast<float*>(work->bytes + 0x90));
    particle_quad_add(u, q, t);
    std::memcpy(work->bytes + 0x90, u, 16);
    std::memcpy(work->bytes + 0x60, u, 16);

    particle_d500_clamp_sub_from_work60(particle_sub, work);

    particle_quad_scale_copy(t, reinterpret_cast<float*>(b + 0x60), f3_scale);
    std::memcpy(work->bytes + 0x90, t, 16);
    std::memcpy(work->bytes + 0x80, t, 16);

    particle_quad_add(q,
                      reinterpret_cast<float*>(work->bytes + 0x00),
                      reinterpret_cast<float*>(work->bytes + 0x80));
    std::memcpy(work->bytes + 0x90, q, 16);
    std::memcpy(work->bytes + 0x70, q, 16);

    particle_quad_scale_copy(t, reinterpret_cast<float*>(work->bytes + 0x50), f1);
    particle_quad_add(q,
                      reinterpret_cast<float*>(work->bytes + 0x70),
                      reinterpret_cast<float*>(work->bytes + 0x90));
    particle_quad_add(u, q, t);
    std::memcpy(work->bytes + 0x60, u, 16);

    particle_d500_clamp_sub_from_work60(particle_sub, work);
}

static void particle_d500_pass(void* basis, void* particle_sub) {
    if (basis == 0 || particle_sub == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    const float kTwo = 2.0f;

    float q[4];
    float t[4];
    float u[4];
    float bc[4];

    /* Phase A @ 0x0036D52C: B0+C0 → sub, then (B0+C0)+D0 min-clamp. */
    particle_quad_add(q,
                      reinterpret_cast<const float*>(b + 0xB0),
                      reinterpret_cast<const float*>(b + 0xC0));
    std::memcpy(particle_sub, q, 16);
    std::memcpy(static_cast<char*>(particle_sub) + 0x10, q, 16);

    std::memcpy(bc, q, 16);
    std::memcpy(t, b + 0xC0, 16);
    particle_quad_scale(t, kTwo);
    particle_quad_add(u, reinterpret_cast<const float*>(b + 0xB0), t);
    particle_quad_add(q, bc, reinterpret_cast<const float*>(b + 0xD0));
    particle_sub_clamp_vec3_min_pair(particle_sub, q);

    /* Phase B @ 0x0036D648: (B0+C0) + D0×2, then max-clamp. */
    particle_quad_add(q,
                      reinterpret_cast<const float*>(b + 0xB0),
                      reinterpret_cast<const float*>(b + 0xC0));
    std::memcpy(t, q, 16);
    std::memcpy(u, b + 0xD0, 16);
    particle_quad_scale(u, kTwo);
    particle_quad_add(q, t, u);
    particle_sub_clamp_vec3_max_pair(particle_sub, q);

    const float f3_scale = particle_d500_pick_scale(b);

    particle_d500_work work;
    std::memset(&work, 0, sizeof(work));
    particle_d500_seed_work(&work, particle_sub);
    particle_d500_pass_basis70_a0(basis, &work);
    particle_d500_pass_basis70_a0_b(basis, &work);
    particle_d500_apply_work_to_sub(basis, &work, particle_sub, f3_scale);
}

void func_0036D500(void* basis, void* particle_sub) {
    if (basis == 0 || particle_sub == 0) {
        return;
    }
    /* EE body is ~0x2514 bytes of unrolled passes (~0x280 bytes per full host pass). */
    for (int pass = 0; pass < 10; pass++) {
        particle_d500_pass(basis, particle_sub);
    }
}
#endif

// Retail @ 0x00370058 — particle basis finalize (GTE-heavy; host covers prologue).
INCLUDE_ASM("mem/rom21_func_00370058", func_00370058);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(func_00370058, ".word 0x27bdfff0\n\t")
#else
void func_00370058(void* basis, void* tmpl, void* param, void* particle_sub) {
    if (basis == 0 || param == 0 || tmpl == 0) {
        return;
    }
    char* b = static_cast<char*>(basis);
    char* prm = static_cast<char*>(param);
    const float* mat = reinterpret_cast<const float*>(tmpl);

    /* Incoming $f12 is usually 0; param+0x10 is the retail scale copied into $f20 at entry. */
    const float f20_entry = *reinterpret_cast<float*>(prm + 0x10);
    float f20 = f20_entry;

    func_0036CE00(basis,
                  *reinterpret_cast<float*>(prm + 0x10),
                  *reinterpret_cast<float*>(prm + 0xCC),
                  *reinterpret_cast<float*>(prm + 0x18));

    const float half_1c = *reinterpret_cast<float*>(prm + 0x1C) * 0.5f;
    const float p14 = *reinterpret_cast<float*>(prm + 0x14);
    func_0036CE28(basis, p14 - half_1c, p14 + half_1c);

    if (f20 < 0.0f) {
        *reinterpret_cast<float*>(b + 0x38) = f20;
    } else {
        func_0036CCB8(basis);
    }

    f20 = 1.0f;
    func_0036D008(basis);

    float quad[4];
    float tmp[4];
    alignas(16) char retail_sp[0x70];
    std::memset(retail_sp, 0, sizeof(retail_sp));

    particle_mat4_mul_vec4(quad, mat,
                           *reinterpret_cast<float*>(prm + 0x24),
                           *reinterpret_cast<float*>(prm + 0x28),
                           *reinterpret_cast<float*>(prm + 0x2C),
                           1.0f);
    particle_gte_store_quad(b, 0xB0, quad, 1);

    particle_mat4_mul_vec4(quad, mat,
                           *reinterpret_cast<float*>(prm + 0x30),
                           *reinterpret_cast<float*>(prm + 0x34),
                           *reinterpret_cast<float*>(prm + 0x38),
                           0.0f);
    particle_gte_store_quad(b, 0xC0, quad, 1);

    particle_mat4_mul_vec4(quad, mat,
                           *reinterpret_cast<float*>(prm + 0x3C),
                           *reinterpret_cast<float*>(prm + 0x40),
                           *reinterpret_cast<float*>(prm + 0x44),
                           0.0f);
    particle_gte_store_quad(b, 0xD0, quad, 0);

    std::memcpy(tmp, b + 0xC0, 16);
    for (int i = 0; i < 4; i++) {
        quad[i] = tmp[i] + *reinterpret_cast<float*>(b + 0xD0 + i * 4);
    }
    const float kOnePointFive = 1.5f;
    for (int i = 0; i < 4; i++) {
        quad[i] *= kOnePointFive;
    }
    std::memcpy(tmp, quad, 16);
    for (int i = 0; i < 4; i++) {
        quad[i] = *reinterpret_cast<float*>(b + 0xB0 + i * 4) - tmp[i];
    }
    particle_gte_store_quad(b, 0xB0, quad, 0);

    const float div_c = *reinterpret_cast<float*>(prm + 0x0C);
    float sp10[4];
    const struct {
        int prm_off;
        int basis_off;
    } mul_rows[] = {
        {0x48, 0x70}, {0x54, 0x80}, {0x60, 0x90}, {0x6C, 0xA0},
    };
    for (const auto& row : mul_rows) {
        particle_mat4_mul_vec4(quad, mat,
                               *reinterpret_cast<float*>(prm + row.prm_off),
                               *reinterpret_cast<float*>(prm + row.prm_off + 4),
                               *reinterpret_cast<float*>(prm + row.prm_off + 8),
                               0.0f);
        particle_gte_vec_scale_uniform(quad, div_c);
        particle_gte_store_quad(b, row.basis_off, quad, 1);
    }

    std::memcpy(tmp, b + 0x80, 16);
    for (int i = 0; i < 4; i++) {
        quad[i] = tmp[i] + *reinterpret_cast<float*>(b + 0x90 + i * 4);
    }
    for (int i = 0; i < 4; i++) {
        quad[i] *= kOnePointFive;
    }
    std::memcpy(tmp, quad, 16);
    for (int i = 0; i < 4; i++) {
        quad[i] = *reinterpret_cast<float*>(b + 0xB0 + i * 4) - tmp[i];
    }
    particle_gte_store_quad(b, 0xB0, quad, 0);

    /* Retail 0x00370430–0x00370480: basis+0x70 -= (0x80+0x90+A0)×param+0x84. */
    particle_quad_add(tmp,
                      reinterpret_cast<float*>(b + 0x80),
                      reinterpret_cast<float*>(b + 0x90));
    particle_quad_add(sp10, tmp, reinterpret_cast<float*>(b + 0xA0));
    particle_quad_scale(sp10, *reinterpret_cast<float*>(prm + 0x84));
    float b70[4];
    std::memcpy(b70, b + 0x70, 16);
    particle_quad_sub(reinterpret_cast<float*>(b + 0x70), b70, sp10);

    particle_mat4_mul_vec4(quad, mat,
                           *reinterpret_cast<float*>(prm + 0x88),
                           *reinterpret_cast<float*>(prm + 0x8C),
                           *reinterpret_cast<float*>(prm + 0x90),
                           0.0f);
    particle_gte_vec_scale_uniform(quad, div_c);
    std::memcpy(tmp, quad, 16);
    particle_quad_add(quad, tmp, reinterpret_cast<float*>(b + 0xA0));
    particle_quad_scale(quad, *reinterpret_cast<float*>(prm + 0x84));
    std::memcpy(retail_sp + 0x00, quad, 16);

    particle_mat4_mul_vec4(quad, mat,
                           *reinterpret_cast<float*>(prm + 0x98),
                           *reinterpret_cast<float*>(prm + 0x9C),
                           *reinterpret_cast<float*>(prm + 0xA0),
                           0.0f);
    particle_gte_vec_scale_uniform(quad, div_c);
    std::memcpy(tmp, quad, 16);
    for (int i = 0; i < 4; i++) {
        quad[i] = tmp[i] + quad[i] * *reinterpret_cast<float*>(prm + 0x94);
    }
    std::memcpy(retail_sp + 0x20, quad, 16);

    particle_mat4_mul_vec4(quad, mat,
                           *reinterpret_cast<float*>(prm + 0xA8),
                           *reinterpret_cast<float*>(prm + 0xAC),
                           *reinterpret_cast<float*>(prm + 0xB0),
                           0.0f);
    for (int i = 0; i < 4; i++) {
        quad[i] = quad[i] + tmp[i] * *reinterpret_cast<float*>(prm + 0xA4);
    }
    std::memcpy(retail_sp + 0x40, quad, 16);
    particle_gte_store_quad(b, 0x40, quad, 0);

    particle_mat4_mul_vec4(quad, mat,
                           *reinterpret_cast<float*>(prm + 0xB8),
                           *reinterpret_cast<float*>(prm + 0xBC),
                           0.0f,
                           0.0f);
    particle_gte_vec_scale_uniform(quad, div_c);
    std::memcpy(retail_sp + 0x60, quad, 16);
    particle_gte_store_quad(b, 0x60, quad, 0);

    f20 = 1.0f;
    float f12 = *reinterpret_cast<float*>(prm + 0x1C) * f20 + *reinterpret_cast<float*>(prm + 0x14);
    func_0036D318(basis, retail_sp, retail_sp + 0x20, retail_sp + 0x40, retail_sp + 0x60, f12);

    const float cmp = *reinterpret_cast<float*>(prm + 0x08);
    if (!(cmp < f12)) {
        f12 = *reinterpret_cast<float*>(prm + 0x1C) * f20 + *reinterpret_cast<float*>(prm + 0x14);
    } else {
        f12 = 0.0f;
    }
    func_0036D3D8(basis, f12);
    func_0036D500(basis, particle_sub);
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif
