#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cGameComm_construct(void);
PS2_RETAIL_ASM_ONLY(
    cGameComm_construct,
    ".word 0x27bdffd0\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x3c050048\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x240405f0\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24a504b8\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0220302d\n\t"
    ".word 0x0c095b1e\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0xaf82fdfc\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
