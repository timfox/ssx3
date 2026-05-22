#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void MUTEX_unlock(void);
PS2_RETAIL_ASM_ONLY(
    MUTEX_unlock,
    ".word 0x27bdffe0\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c108f24\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x8e030004\n\t"
    ".word 0x14620007\n\t"
    ".word 0x3c020045\n\t"
    ".word 0x8e020008\n\t"
    ".word 0x2442ffff\n\t"
    ".word 0x1440000c\n\t"
    ".word 0xae020008\n\t"
    ".word 0xae000004\n\t"
    ".word 0x3c020045\n\t"
    ".word 0x8c430de4\n\t"
    ".word 0x10600005\n\t"
    ".word 0x00000000\n\t"
    ".word 0x0c108f74\n\t"
    ".word 0x8e04000c\n\t"
    ".word 0x10000004\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0x0c108f70\n\t"
    ".word 0x8e04000c\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
