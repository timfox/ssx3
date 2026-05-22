#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

void cBxPseudoRng_Seed(uint* param_1, uint seedAddend);
uint cBxPseudoRng_NextInt(uint* state);

extern uint D_004FF018[];
extern uint D_004FF030[];
extern unsigned long g_cMemMan_default_flags;
void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);

//100%
//https://decomp.me/scratch/ViFdz
INCLUDE_ASM("bxrandom", BXsrand__FUi);
#ifdef SKIP_ASM
void BXsrand(uint seed) {
    cBxPseudoRng_Seed(reinterpret_cast<uint*>(D_004FF018), seed);
}
#endif

INCLUDE_ASM("bxrandom", BXrand__Fv);
#ifdef SKIP_ASM
void BXrand() {
    (void)cBxPseudoRng_NextInt(D_004FF018);
}
#endif

INCLUDE_ASM("bxrandom", AIrand);
#ifdef SKIP_ASM
unsigned int AIrand() {
    return cBxPseudoRng_NextInt(D_004FF030);
}
#endif
INCLUDE_ASM("bxrandom", AIrandf__Fff);
#ifdef SKIP_ASM
float AIrandf(float a, float b) {
    unsigned int r = (AIrand() & 0x7FFFFFFF) | 0x3F800000;
    float f = *reinterpret_cast<float*>(&r) - 1.0f;
    return a + (b - a) * f;
}
#endif

INCLUDE_ASM("bxrandom", func_00317890__Fff);
#ifdef SKIP_ASM
float func_00317890(float param_1, float param_2) {
    const float fVar1 = AIrandf(-1.0f, 1.0f);
    return param_1 + param_2 * fVar1;
}
#endif

extern "C" void* func_003E6574(void* a0, void* heap_tag, int size);

INCLUDE_ASM("bxrandom", func_003178E0);
#ifdef SKIP_ASM
void* func_003178E0(void) {
    return func_003E6574(0, reinterpret_cast<void*>(0x004FF030u), 0x18);
}
#endif

INCLUDE_ASM("bxrandom", func_00317908);
#ifdef SKIP_ASM
void* func_00317908(void* /*heap_tag*/) {
    return func_003E6574(0, reinterpret_cast<void*>(0x004FF030u), 0x18);
}
#endif

INCLUDE_ASM("bxrandom", func_00317930);
#ifdef SKIP_ASM
void* func_00317930(void) {
    return func_003E6574(0, reinterpret_cast<void*>(0x0049DCB0u), 0x18);
}
#endif

#ifdef SKIP_ASM
void* operator_new(unsigned long size) {
    return cMemMan_alloc(size,
                       reinterpret_cast<void*>(0x004A3E88u),
                       g_cMemMan_default_flags,
                       0);
}
#endif

INCLUDE_ASM("bxrandom", cBxPseudoRng_Seed);
#ifdef SKIP_ASM
void cBxPseudoRng_Seed(uint* state, uint seedAddend) {
    uint hi = 0;
    uint lo = seedAddend;

    const uint kConsts[] = {
        0xF22D0E56u,
        0x96041893u,
        0x3DF3B646u,
        0x40DDE76Du,
        0x97327AE1u,
        0xD1A9FBE7u,
    };

    for (int i = 0; i < 5; ++i) {
        const uint old_lo = lo;
        lo += kConsts[i];
        hi += (lo < old_lo) ? 1u : 0u;
        state[i] = hi;
    }

    const uint old_lo = lo;
    lo += kConsts[5];
    hi += (lo < old_lo) ? 1u : 0u;
    (void)hi;
    state[5] = lo;
}
#endif

INCLUDE_ASM("bxrandom", cBxPseudoRng_NextInt);
#ifdef SKIP_ASM
uint cBxPseudoRng_NextInt(uint* st) {
    uint v2 = st[5];
    uint v3 = st[4];
    uint v7 = v2 + v3;
    uint v9;

    if (v7 < v2) {
        v9 = 1;
    } else {
        v9 = (v7 < v3) ? 1u : 0u;
    }

    v3 = st[3];
    const uint v5 = st[2];
    v2 = v7 + v3;
    st[4] = v7;
    v7 = v2 + v9;
    v9 = (v7 < v3) ? 1u : 0u;

    const uint v6 = st[1];
    v2 = v7 + v5;
    st[3] = v7;
    v7 = v2 + v9;
    v9 = (v7 < v5) ? 1u : 0u;

    uint v4 = st[0];
    const uint v1 = v7 + v6;
    st[2] = v7;
    v7 = v1 + v9;
    v9 = (v7 < v6) ? 1u : 0u;

    v2 = st[5];
    v4 = v7 + v4;
    v9 = (v7 < v6) ? 1u : 0u;
    st[1] = v7;
    v7 = v4 + v9;
    v2 = v2 + 1u;
    st[5] = v2;
    if (v2 != 0u) {
        st[0] = v7;
        return v7;
    }

    v2 = st[4] + 1u;
    st[4] = v2;
    if (v2 != 0u) {
        st[0] = v7;
        return v7;
    }

    v2 = st[3] + 1u;
    st[3] = v2;
    if (v2 != 0u) {
        st[0] = v7;
        return v7;
    }

    v2 = st[2] + 1u;
    st[2] = v2;
    if (v2 != 0u) {
        st[0] = v7;
        return v7;
    }

    v2 = st[1] + 1u;
    st[1] = v2;
    if (v2 != 0u) {
        st[0] = v7;
        return v7;
    }

    v2 = v7 + 1u;
    st[0] = v2;
    return v2;
}
#endif

#ifdef SSX3_HOST
} // extern "C"
#endif
