#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* func_002D1BD8(void);
#ifdef SKIP_ASM
float cSpline_calcLength(void* spline);
PS2_RETAIL_ASM_ONLY(
    cSpline_calcLength,
    ".word 0x8c820008\n\t"
    ".word 0x24040001\n\t"
    ".word 0x8c430068\n\t"
    ".word 0x8c650020\n\t"
    ".word 0x0085102a\n\t"
    ".word 0x10400009\n\t"
    ".word 0x8c630024\n\t"
    ".word 0x24a4ffff\n\t"
    ".word 0x2484ffff\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x1480fffa\n\t"
    ".word 0x8c630064\n\t"
    ".word 0xc461000c\n\t"
    ".word 0xc4600084\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x46010000\n\t"
    ".word 0x00000000\n\t"
)
#endif
