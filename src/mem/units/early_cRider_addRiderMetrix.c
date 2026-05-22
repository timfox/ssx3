#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cRider_addRiderMetrix(void);
PS2_RETAIL_ASM_ONLY(
    cRider_addRiderMetrix,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c050045\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24a57970\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x240401cc\n\t"
    ".word 0x0c045c92\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0200282d\n\t"
    ".word 0xae020790\n\t"
    ".word 0x0c045cb8\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
)
#endif
