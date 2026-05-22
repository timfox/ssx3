#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cPSPGraphicsMan_NewBindTexID(void);
PS2_RETAIL_ASM_ONLY(
    cPSPGraphicsMan_NewBindTexID,
    ".word 0x27bdffd0\n\t"
    ".word 0x00051080\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x26300008\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x02028021\n\t"
    ".word 0x24a52078\n\t"
    ".word 0x24040058\n\t"
    ".word 0x3c062100\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x0000382d\n\t"
    ".word 0xae020000\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x8e220000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x24420001\n\t"
    ".word 0xae220000\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#endif
