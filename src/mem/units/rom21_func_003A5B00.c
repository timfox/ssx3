#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A5B00(void);
PS2_RETAIL_ASM_ONLY(
    func_003A5B00,
    ".word 0x03e00008\n\t"
    ".word 0x80820044\n\t"
)
#endif
