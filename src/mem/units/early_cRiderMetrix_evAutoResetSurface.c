#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cRiderMetrix_evAutoResetSurface(void);
PS2_RETAIL_ASM_ONLY(
    cRiderMetrix_evAutoResetSurface,
    ".word 0x27bdfff0\n\t"
    ".word 0x3c013fc0\n\t"
    ".word 0x44816000\n\t"
    ".word 0x3c060045\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24c67888\n\t"
    ".word 0x24050033\n\t"
    ".word 0x0c045eba\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x44800000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
    ".word 0x00000000\n\t"
)
#endif
