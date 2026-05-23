#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A5830(void);
PS2_RETAIL_ASM_ONLY(
    func_003A5830,
    ".word 0x03e00008\n\t"
    ".word 0x90820015\n\t"
)
#endif
