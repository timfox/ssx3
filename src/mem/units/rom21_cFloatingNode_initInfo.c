#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern char D_0049E890[];
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void cInstanceNode_getBoundBoxInfo(void* self, void* out, unsigned long flags);
extern void cFloatingNode_initInfo_impl(void* self);
#ifdef SKIP_ASM
void cFloatingNode_initInfo(void* self);
PS2_RETAIL_ASM_ONLY(
    cFloatingNode_initInfo,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24a5e890\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040030\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x0200202d\n\t"
    ".word 0xae020078\n\t"
    ".word 0x0c0d415c\n\t"
    ".word 0x24a60020\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
