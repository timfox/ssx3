#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cRider_initOnce(void);
PS2_RETAIL_ASM_ONLY(
    cRider_initOnce,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x10a00005\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0470a6\n\t"
    ".word 0x00000000\n\t"
    ".word 0x1000000d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x8e030880\n\t"
    ".word 0x24020007\n\t"
    ".word 0x10620008\n\t"
    ".word 0x24110007\n\t"
    ".word 0x0c0470a6\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0c0f94ac\n\t"
    ".word 0x24040001\n\t"
    ".word 0x8e020880\n\t"
    ".word 0x1451fffa\n\t"
    ".word 0x00000000\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
