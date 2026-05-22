#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cReplay_restoreObject(void);
PS2_RETAIL_ASM_ONLY(
    cReplay_restoreObject,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x00a0802d\n\t"
    ".word 0x0c09ba9a\n\t"
    ".word 0x0220202d\n\t"
    ".word 0x0040182d\n\t"
    ".word 0x2c620017\n\t"
    ".word 0x10400109\n\t"
    ".word 0x3c020048\n\t"
    ".word 0x00031880\n\t"
    ".word 0x244216c0\n\t"
    ".word 0x00621821\n\t"
    ".word 0x8c640000\n\t"
    ".word 0x00800008\n\t"
    ".word 0x00000000\n\t"
)
#endif
