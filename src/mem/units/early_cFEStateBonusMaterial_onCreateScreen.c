#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cFEStateBonusMaterial_onCreateScreen(void);
PS2_RETAIL_ASM_ONLY(
    cFEStateBonusMaterial_onCreateScreen,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x3c040046\n\t"
    ".word 0x24840168\n\t"
    ".word 0x0c0c5d9c\n\t"
    ".word 0x8e110010\n\t"
    ".word 0x0220202d\n\t"
    ".word 0x0040302d\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0c0e5f30\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x10400005\n\t"
    ".word 0xae020040\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x0c0e721c\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#endif
