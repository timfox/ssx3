#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_00340A10(void);
PS2_RETAIL_ASM_ONLY(
    func_00340A10,
    ".word 0x03e00008\n\t"
    ".word 0xc48000a0\n\t"
)
#endif
