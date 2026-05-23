#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A6180(void);
PS2_RETAIL_ASM_ONLY(
    func_003A6180,
    ".word 0x03e00008\n\t"
    ".word 0x8c82007c\n\t"
)
#endif
