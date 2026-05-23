#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B03F0(void);
PS2_RETAIL_ASM_ONLY(
    func_003B03F0,
    ".word 0x03e00008\n\t"
    ".word 0xa0850024\n\t"
)
#endif
