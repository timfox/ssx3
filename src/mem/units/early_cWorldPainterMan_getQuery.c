#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cWorldPainterMan_getQuery(void);
PS2_RETAIL_ASM_ONLY(
    cWorldPainterMan_getQuery,
    ".word 0x27bdffe0\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x00a0802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x3c050048\n\t"
    ".word 0x24040018\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24a53de8\n\t"
    ".word 0x8e050000\n\t"
    ".word 0x0c0b0102\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
