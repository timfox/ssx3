#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void delete(void);
PS2_RETAIL_ASM_ONLY(
    operator_delete__FPi,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x3c100053\n\t"
    ".word 0x26108b00\n\t"
    ".word 0x0080882d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0f95c0\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0c0c66d2\n\t"
    ".word 0x0220202d\n\t"
    ".word 0x0c0f95d8\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
