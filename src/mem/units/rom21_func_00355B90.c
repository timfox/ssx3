#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* cBucketMan_first(void* bucket_man, int bucket_id);
extern void* cBucketMan_next(void* bucket_man, void* node, int bucket_id);
extern void cMoveNode_addModifierBlock(void* self, void* param);
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void* tModifierBlock_tModifierBlock(void* block, void* param);
extern char D_0049E9D8[];
extern char D_0049E940[];
typedef void (*NodeConfigBindFn)(void* src, void* dst, int count);
extern void func_0035F6E8(void* spline, void* config);
#ifdef SKIP_ASM
void* func_00355B90(void* spline, void* config);
PS2_RETAIL_ASM_ONLY(
    func_00355B90,
    ".word 0x27bdffd0\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x00a0802d\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x3c050049\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040050\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24a5e940\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0c0d7dba\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0220202d\n\t"
    ".word 0x0c0d5554\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#endif
