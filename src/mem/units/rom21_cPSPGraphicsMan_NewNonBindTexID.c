#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cPSPGraphicsMan_NewNonBindTexID(void);
PS2_RETAIL_ASM_ONLY(
    cPSPGraphicsMan_NewNonBindTexID,
    ".word 0x27bdffd0\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x24a52068\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0000382d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24910008\n\t"
    ".word 0x8c821f48\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x2450fff8\n\t"
    ".word 0x8c460000\n\t"
    ".word 0x02048023\n\t"
    ".word 0x24630001\n\t"
    ".word 0x00108083\n\t"
    ".word 0xac861f48\n\t"
    ".word 0x00101080\n\t"
    ".word 0xac830004\n\t"
    ".word 0x02228821\n\t"
    ".word 0x24040058\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x3c062100\n\t"
    ".word 0xae220000\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#endif
