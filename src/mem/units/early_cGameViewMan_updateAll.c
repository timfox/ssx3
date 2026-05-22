#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cGameViewMan_updateAll(void);
PS2_RETAIL_ASM_ONLY(
    cGameViewMan_updateAll,
    ".word 0x27bdffc0\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x0080902d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8e420010\n\t"
    ".word 0x1040000f\n\t"
    ".word 0x0000882d\n\t"
    ".word 0x26500004\n\t"
    ".word 0x8e050000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x26310001\n\t"
    ".word 0x26100004\n\t"
    ".word 0x8ca20090\n\t"
    ".word 0x84440018\n\t"
    ".word 0x8c43001c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x8e420010\n\t"
    ".word 0x0222102b\n\t"
    ".word 0x5440fff6\n\t"
    ".word 0x8e050000\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
    ".word 0x00000000\n\t"
)
#endif
