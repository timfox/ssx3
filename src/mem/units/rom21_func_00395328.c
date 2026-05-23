#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_00395328(void);
PS2_RETAIL_ASM_ONLY(
    func_00395328,
    ".word 0x03e00008\n\t"
    ".word 0x8c8259c8\n\t"
)
#endif
