#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B0400(void);
PS2_RETAIL_ASM_ONLY(
    func_003B0400,
    ".word 0x03e00008\n\t"
    ".word 0xa0850026\n\t"
)
#endif
