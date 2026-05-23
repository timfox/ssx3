#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B03C0(void);
PS2_RETAIL_ASM_ONLY(
    func_003B03C0,
    ".word 0x03e00008\n\t"
    ".word 0xac85000c\n\t"
)
#endif
