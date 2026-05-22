#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* cBucketMan_first(void* bucket_man, int bucket_id);
extern void* func_003D5290(void* query_ptr);
extern int func_003D3030(void* block);
extern void func_003D11B8(void* block, void* scratch);
extern int D_00517644;
extern int D_00517668;
#ifdef SKIP_ASM
void* func_003D53B8(int shift_heap, int shift_block);
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
#endif
