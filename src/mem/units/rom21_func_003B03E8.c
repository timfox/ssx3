#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B03E8(void);
PS2_RETAIL_ASM_ONLY(
    func_003B03E8,
    ".word 0x03e00008\n\t"
    ".word 0xac850020\n\t"
)
#endif
