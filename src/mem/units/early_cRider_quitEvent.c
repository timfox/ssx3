#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cRider_quitEvent(void);
PS2_RETAIL_ASM_ONLY(
    cRider_quitEvent,
    ".word 0x27bdffe0\n\t"
    ".word 0x44800800\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0xc4800470\n\t"
    ".word 0x46000836\n\t"
    ".word 0x00000000\n\t"
    ".word 0x45010002\n\t"
    ".word 0x24020001\n\t"
    ".word 0x0000102d\n\t"
    ".word 0x1440000f\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0x24100001\n\t"
    ".word 0x8c8306c0\n\t"
    ".word 0xac900480\n\t"
    ".word 0x248506c0\n\t"
    ".word 0x84640038\n\t"
    ".word 0x8c62003c\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x3c030053\n\t"
    ".word 0x00021080\n\t"
    ".word 0x246366d0\n\t"
    ".word 0x00431021\n\t"
    ".word 0xac500000\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
