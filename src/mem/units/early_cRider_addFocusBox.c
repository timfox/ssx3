#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cRider_addFocusBox(void);
PS2_RETAIL_ASM_ONLY(
    cRider_addFocusBox,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c050045\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24a57960\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040010\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x8f84f7b8\n\t"
    ".word 0x0040282d\n\t"
    ".word 0xae02078c\n\t"
    ".word 0x8c820084\n\t"
    ".word 0x8c43000c\n\t"
    ".word 0x0c040cec\n\t"
    ".word 0x8c6400a4\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
