#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cSectionMan_setSky(void);
PS2_RETAIL_ASM_ONLY(
    cSectionMan_setSky,
    ".word 0x27bdfff0\n\t"
    ".word 0x2ca20016\n\t"
    ".word 0x1040001c\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x3c020048\n\t"
    ".word 0x00051880\n\t"
    ".word 0x2442b4e0\n\t"
    ".word 0x00621821\n\t"
    ".word 0x8c640000\n\t"
    ".word 0x00800008\n\t"
    ".word 0x00000000\n\t"
)
#endif
