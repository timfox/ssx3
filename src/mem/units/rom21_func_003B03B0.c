#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B03B0(void);
PS2_RETAIL_ASM_ONLY(
    func_003B03B0,
    ".word 0x03e00008\n\t"
    ".word 0xac850004\n\t"
)
#endif
