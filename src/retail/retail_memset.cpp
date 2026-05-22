// Portable reading of retail func_003E6448 @ 0x2E7448.
// EE matching uses src/mem/units/rom21_func_003E6448.c (embedded opcodes).

#include "retail/synctask.h"

#include <stdint.h>

extern "C" {

void retail_memset_u8(void* dst, unsigned char fill, unsigned int size) {
    auto* p = static_cast<unsigned char*>(dst);
    unsigned int n = size;
    if (n == 0) {
        return;
    }

    const unsigned int pattern = (unsigned int)fill * 0x01010101u;

    if (((uintptr_t)p & 1u) == 0) {
        while (n >= 2) {
            p[0] = fill;
            p[1] = fill;
            p += 2;
            n -= 2;
        }
    }

    if (((uintptr_t)p & 3u) == 0) {
        while (n >= 4) {
            *reinterpret_cast<unsigned int*>(p) = pattern;
            p += 4;
            n -= 4;
        }
    }

    while (n >= 64) {
        for (int i = 0; i < 64; i += 4) {
            *reinterpret_cast<unsigned int*>(p + i) = pattern;
        }
        p += 64;
        n -= 64;
    }

    while (n >= 16) {
        *reinterpret_cast<unsigned int*>(p + 0) = pattern;
        *reinterpret_cast<unsigned int*>(p + 4) = pattern;
        *reinterpret_cast<unsigned int*>(p + 8) = pattern;
        *reinterpret_cast<unsigned int*>(p + 12) = pattern;
        p += 16;
        n -= 16;
    }

    while (n >= 4) {
        *reinterpret_cast<unsigned int*>(p) = pattern;
        p += 4;
        n -= 4;
    }

    if (((uintptr_t)p & 1u) == 0) {
        while (n >= 2) {
            p[0] = fill;
            p[1] = fill;
            p += 2;
            n -= 2;
        }
    }

    if (n > 0) {
        p[0] = fill;
    }
}

} // extern "C"
