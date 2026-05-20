#include "platform/host.h"

/*
 * Host stand-ins for bxrandom BSS and functions still in asm on PS2.
 * Remove when cBxPseudoRng_* / AIrand are decompiled and linked from src/bxrandom.cpp.
 */

uint D_004FF018[4] = {0x12345678u, 0, 0, 0};

void cBxPseudoRng_Seed(uint* state, uint seedAddend) {
    if (!state) {
        return;
    }
    state[0] = seedAddend;
    state[1] = seedAddend ^ 0xA5A5A5A5u;
    state[2] = seedAddend + 1u;
    state[3] = 0;
}

void cBxPseudoRng_NextInt(const uint* state) {
    uint* mut = const_cast<uint*>(state);
    if (!mut) {
        return;
    }
    mut[0] = mut[0] * 1664525u + 1013904223u;
    mut[1] ^= mut[0];
    mut[2] += 0x6C078965u;
}

unsigned int AIrand() {
    cBxPseudoRng_NextInt(D_004FF018);
    return D_004FF018[0];
}
