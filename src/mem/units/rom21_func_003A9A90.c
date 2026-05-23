#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A9A90(void);
PS2_RETAIL_ASM_ONLY(
    func_003A9A90,
    ".word 0x03e00008\n\t"
    ".word 0x24820220\n\t"
)
#endif
