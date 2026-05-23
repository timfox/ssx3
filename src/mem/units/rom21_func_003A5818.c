#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A5818(void);
PS2_RETAIL_ASM_ONLY(
    func_003A5818,
    ".word 0x03e00008\n\t"
    ".word 0xa0850014\n\t"
)
#endif
