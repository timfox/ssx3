#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void new_global(void);
PS2_RETAIL_ASM_ONLY(
    operator_new,
    ".word 0x27bdfff0\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x00000000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
)
#endif
