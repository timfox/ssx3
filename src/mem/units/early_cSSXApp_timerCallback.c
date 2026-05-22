#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cSSXApp_timerCallback(void);
PS2_RETAIL_ASM_ONLY(
    cSSXApp_timerCallback,
    ".word 0x27bdfff0\n\t"
    ".word 0x8f84f7b0\n\t"
    ".word 0x10800003\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0c9ae2\n\t"
    ".word 0x00000000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
    ".word 0x00000000\n\t"
)
#endif
