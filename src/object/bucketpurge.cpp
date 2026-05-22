#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

extern void* cBucketMan_first(void* bucket_man, int bucket_id);

// Retail type live-count table (rom21 @ 0x00517644).
extern int D_00517644;
extern int D_00517668;

static const int kBucketStride = 0x44; /* rom21_func_003D5330 unit */

// Retail @ 0x003D5330 — decrement per-type counts for nodes in one bucket.
INCLUDE_ASM("mem/rom21_func_003D5330", func_003D5330);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_003D5330,
    ".word 0x3c020051\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x244d7644\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x25a90024\n\t"
    ".word 0x3c0c0100\n\t"
    ".word 0x240b0928\n\t"
    ".word 0x240a0928\n\t"
    ".word 0x00ac1004\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00821024\n\t"
    ".word 0x1040000f\n\t"
    ".word 0x24a80001\n\t"
    ".word 0x00ab1818\n\t"
    ".word 0x006d1021\n\t"
    ".word 0x8c430000\n\t"
    ".word 0x1060000a\n\t"
    ".word 0x00aa1018\n\t"
    ".word 0x24060017\n\t"
    ".word 0x00492821\n\t"
    ".word 0x8ca30000\n\t"
    ".word 0x24e20001\n\t"
    ".word 0x24a50004\n\t"
    ".word 0x24c6ffff\n\t"
    ".word 0x00000000\n\t"
    ".word 0x04c1fffa\n\t"
    ".word 0x0043380b\n\t"
    ".word 0x0100282d\n\t"
    ".word 0x28a20004\n\t"
    ".word 0x1440ffec\n\t"
    ".word 0x00ac1004\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x00e0102d\n\t"
    ".word 0x00000000\n\t"
)
#else
#if defined(SSX3_HOST)
void func_003D5330(void* bucket_man, int bucket_id) {
    const int slot = bucket_id & D_00517668;
    if (slot == 0) {
        return;
    }

    char* pool = *(char**)((char*)bucket_man + 4);
    char* bucket = pool + (slot * 0x44);
    void* node = cBucketMan_first(bucket_man, bucket_id);
    char* sentinel = bucket + 0x14;

    while (node != 0 && node != sentinel) {
        const int type_index = *(unsigned short*)((char*)node + 0x10);
        int* counter = (int*)((char*)&D_00517644 + (type_index * 4));
        if (*counter > 0) {
            *counter -= 1;
        }
        node = *(void**)((char*)node + 4);
    }
}
#endif /* SSX3_HOST */
#endif /* !SSX3_HOST */
#endif /* SKIP_ASM */

// Retail @ 0x003D53B8 — bucket teardown helper (calls retail alloc/free helpers).
INCLUDE_ASM("mem/rom21_func_003D53B8", func_003D53B8);

#ifdef SSX3_HOST
}
#endif
