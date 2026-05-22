#include "common.h"
#include <cmath>
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

extern void func_003C5F5C(void* out, void* a, void* b);

static void rail_store_identity(float* matrix) {
    std::memset(matrix, 0, 16 * sizeof(float));
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;
}

static void rail_apply_point_to_basis(const float* basis, float x, float y, float z, float* out) {
    if (basis != nullptr) {
        std::memcpy(out, basis, 0x40);
    } else {
        rail_store_identity(out);
    }
    out[12] = basis ? (basis[12] + x) : x;
    out[13] = basis ? (basis[13] + y) : y;
    out[14] = basis ? (basis[14] + z) : z;
}

// Retail @ 0x0035C0E8 — rail modifier world xform (large; ObjDiff unit).
INCLUDE_ASM("mem/rom21_cRailModifier", cRailModifier_buildXform);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void cRailModifier_buildXform(void* self) {
    if (self == 0) {
        return;
    }

    float basis[16];
    std::memcpy(basis, (char*)self + 0x40, 0x40);

    void* table = *(void**)((char*)self + 0x34);
    int count = *(int*)((char*)self + 0x0C);
    if (count <= 0) {
        count = 2;
    }
    if (count > 16) {
        count = 16;
    }

    const int rail_flags = *(int*)((char*)self + 0x80);
    if (rail_flags >= 0) {
        rail_apply_point_to_basis(basis, 0.0f, 0.0f, 0.0f, (float*)((char*)self + 0x54));
        std::memcpy((char*)self + 0x60, (char*)self + 0x54, 0x40);
        std::memcpy((char*)self + 0x70, (char*)self + 0x54, 0x40);
        *(int*)((char*)self + 0x44) = -1;
        return;
    }

    for (int i = 0; i < count; i++) {
        float px = 0.0f;
        float py = 0.0f;
        float pz = 0.0f;
        if (table != 0) {
            char* point = static_cast<char*>(table) + (i * 0x10);
            px = *(float*)(point + 0);
            py = *(float*)(point + 4);
            pz = *(float*)(point + 8);
        } else {
            px = static_cast<float>(i);
        }

        float* out = (float*)((char*)self + 0x54 + (i * 0x40));
        rail_apply_point_to_basis(basis, px, py, pz, out);
        func_003C5F5C(out, out, basis);
    }

    *(int*)((char*)self + 0x44) = -1;
    *(int*)((char*)self + 0x0C) = count;
    *(int*)((char*)self + 8) = *(int*)((char*)self + 0x34);
}
#endif

#ifdef SSX3_HOST
}
#endif
