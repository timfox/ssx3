#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cInputMap_init(void);
PS2_RETAIL_ASM_ONLY(
    cInputMap_init,
    ".word 0x27bdffe0\n\t"
    ".word 0x00a0102d\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x00022080\n\t"
    ".word 0x24a5dd00\n\t"
    ".word 0x0c0c5f8c\n\t"
    ".word 0xae020000\n\t"
    ".word 0x8e060000\n\t"
    ".word 0x3c05ffff\n\t"
    ".word 0xae020004\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x00063080\n\t"
    ".word 0x0c0f9912\n\t"
    ".word 0x34a5ffff\n\t"
    ".word 0xae000010\n\t"
    ".word 0xae000014\n\t"
    ".word 0xae00000c\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
)
#endif
