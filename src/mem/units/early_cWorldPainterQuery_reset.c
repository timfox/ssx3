#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cWorldPainterQuery_reset(void);
PS2_RETAIL_ASM_ONLY(
    cWorldPainterQuery_reset,
    ".word 0x8f830790\n\t"
    ".word 0x1060000b\n\t"
    ".word 0x00000000\n\t"
    ".word 0xc780bd74\n\t"
    ".word 0x8c620000\n\t"
    ".word 0x00000000\n\t"
    ".word 0xe4400000\n\t"
    ".word 0x8c630014\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x5460fffa\n\t"
    ".word 0x8c620000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
)
#endif
