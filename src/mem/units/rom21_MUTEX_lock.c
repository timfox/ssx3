#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void MUTEX_lock(void);
PS2_RETAIL_ASM_ONLY(
    MUTEX_lock,
    ".word 0x27bdffe0\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c108f24\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x8e030004\n\t"
    ".word 0x14620005\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8e020008\n\t"
    ".word 0x24420001\n\t"
    ".word 0x10000009\n\t"
    ".word 0xae020008\n\t"
    ".word 0x0c108f78\n\t"
    ".word 0x8e04000c\n\t"
    ".word 0x0c108f24\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8e030008\n\t"
    ".word 0xae020004\n\t"
    ".word 0x24630001\n\t"
    ".word 0xae030008\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
)
#endif
