#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cListNode_removeFromList(void* node);
PS2_RETAIL_ASM_ONLY(
    cListNode_removeFromList,
    ".word 0x8c820000\n\t"
    ".word 0x10400006\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x10440004\n\t"
    ".word 0x00000000\n\t"
    ".word 0x54640002\n\t"
    ".word 0xac430004\n\t"
    ".word 0xac420004\n\t"
    ".word 0x50600007\n\t"
    ".word 0xac840000\n\t"
    ".word 0x50640005\n\t"
    ".word 0xac840000\n\t"
    ".word 0x54440002\n\t"
    ".word 0xac620000\n\t"
    ".word 0xac630000\n\t"
    ".word 0xac840000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac840004\n\t"
)
#endif
