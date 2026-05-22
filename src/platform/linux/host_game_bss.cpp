#include "platform/host.h"

/*
 * Host BSS for bxrandom globals not yet modeled in a dedicated TU.
 * cBxPseudoRng_* / AIrand are decompiled in src/bxrandom.cpp (SKIP_ASM).
 */

uint D_004FF018[6] = {};
uint D_004FF030[6] = {};

unsigned long g_cMemMan_default_flags = 0x20000000u;
int g_crowd_frame_tick = 0;
int D_00445DD0 = 0;
