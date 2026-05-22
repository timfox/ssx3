#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
#include "platform/host_heap_synthetic.h"
#endif

#ifdef SSX3_HOST
extern "C" {
#endif

extern void* cBucketMan_first(void* bucket_man, int bucket_id);
extern void* func_003D5290(void* query_ptr);
extern int func_003D3030(void* block);
extern void func_003D11B8(void* block, void* scratch);

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
void func_003D5330(void* bucket_man, int bucket_id) {
    const int slot = bucket_id & D_00517668;
    if (slot == 0) {
        return;
    }

    char* pool = *(char**)((char*)bucket_man + 4);
    char* bucket = pool + (slot * kBucketStride);
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
#endif
#endif

// Retail @ 0x003D53B8 — heap/block pointer setup + init/teardown pair (3030/11B8).
INCLUDE_ASM("mem/rom21_func_003D53B8", func_003D53B8);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_003D53B8,
    ".word 0x27bdff70\n\t"
    ".word 0x3c020100\n\t"
    ".word 0x7fb10070\n\t"
    ".word 0x7fb20060\n\t"
    ".word 0x24110001\n\t"
    ".word 0x7fb30050\n\t"
    ".word 0x00b19004\n\t"
    ".word 0x7fb40040\n\t"
    ".word 0x00829804\n\t"
    ".word 0x7fb00080\n\t"
    ".word 0x3c141000\n\t"
    ".word 0xffbf0030\n\t"
    ".word 0x02341004\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00531025\n\t"
    ".word 0x00528021\n\t"
    ".word 0x0c0f4c0c\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x04400006\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0c0f446e\n\t"
    ".word 0x03a0282d\n\t"
    ".word 0x8fa30000\n\t"
    ".word 0x04610005\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x2631ffff\n\t"
    ".word 0x0621fff3\n\t"
    ".word 0x02341004\n\t"
    ".word 0x0000102d\n\t"
    ".word 0x7bb00080\n\t"
    ".word 0x7bb10070\n\t"
    ".word 0x7bb20060\n\t"
    ".word 0x7bb30050\n\t"
    ".word 0x7bb40040\n\t"
    ".word 0xdfbf0030\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0090\n\t"
    ".word 0x00000000\n\t"
)
#else
static void* bucketpurge_make_block_ptr(int shift_heap, int shift_block) {
    const unsigned lo = 0x01000000u << (unsigned)shift_heap;
    const unsigned hi = 0x20000000u | lo;
    const unsigned offset = 1u << (unsigned)shift_block;
    const unsigned encoded = hi + offset;
#ifdef SSX3_HOST
    void* block = host_heap_synthetic_from_encoded(encoded);
    return block != 0 ? block : (void*)(unsigned long)encoded;
#else
    return (void*)(unsigned long)encoded;
#endif
}

void* func_003D53B8(int shift_heap, int shift_block) {
    void* block = bucketpurge_make_block_ptr(shift_heap, shift_block);
    for (int pass = 1; pass >= 0; pass--) {
        const int status = func_003D3030(block);
        if (status < 0) {
            continue;
        }
        int scratch = -1;
        func_003D11B8(block, &scratch);
        if (scratch >= 0) {
            return block;
        }
    }
    return 0;
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif
