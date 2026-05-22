#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cWorldPainterQuery_cWorldPainterQuery(void);
PS2_RETAIL_ASM_ONLY(
    cWorldPainterQuery_cWorldPainterQuery,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c020048\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24423e00\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0xa2050010\n\t"
    ".word 0x00052a02\n\t"
    ".word 0xc780bd78\n\t"
    ".word 0x92030010\n\t"
    ".word 0x00052a00\n\t"
    ".word 0xae020004\n\t"
    ".word 0x00651825\n\t"
    ".word 0xe600000c\n\t"
    ".word 0x00032202\n\t"
    ".word 0xae030010\n\t"
    ".word 0x2c82000e\n\t"
    ".word 0x10400093\n\t"
    ".word 0xe6000008\n\t"
    ".word 0x3c020048\n\t"
    ".word 0x00041880\n\t"
    ".word 0x24423db0\n\t"
    ".word 0x00621821\n\t"
    ".word 0x8c640000\n\t"
    ".word 0x00800008\n\t"
    ".word 0x00000000\n\t"
)
#endif
