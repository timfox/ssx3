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
int func_00355420(void* self, void* param);
PS2_RETAIL_ASM_ONLY(
    func_00355420,
    ".word 0x27bdfff0\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8c83001c\n\t"
    ".word 0x1060000b\n\t"
    ".word 0x24020001\n\t"
    ".word 0x8c650000\n\t"
    ".word 0x10a00009\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x8ca30000\n\t"
    ".word 0x84640040\n\t"
    ".word 0x8c620044\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x10000002\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
)
#endif
