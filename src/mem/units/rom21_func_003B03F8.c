#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B03F8(void);
PS2_RETAIL_ASM_ONLY(
    func_003B03F8,
    ".word 0x03e00008\n\t"
    ".word 0xa0850025\n\t"
)
#endif
