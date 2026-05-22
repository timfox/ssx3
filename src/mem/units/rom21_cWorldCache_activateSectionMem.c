#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cWorldCache_activateSectionMem(void);
PS2_RETAIL_ASM_ONLY(
    cWorldCache_activateSectionMem,
    ".word 0x27bdffc0\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x00a0802d\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0080902d\n\t"
    ".word 0x0c0e9ec6\n\t"
    ".word 0x8e44000c\n\t"
    ".word 0x8e420008\n\t"
    ".word 0x00108880\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x02228821\n\t"
    ".word 0x24a54e60\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x240400e8\n\t"
    ".word 0x24030058\n\t"
    ".word 0x8e460000\n\t"
    ".word 0x02038018\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x8cc50014\n\t"
    ".word 0x00b02821\n\t"
    ".word 0x0c0e9f22\n\t"
    ".word 0x24a50020\n\t"
    ".word 0xae220000\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
    ".word 0x00000000\n\t"
)
#endif
