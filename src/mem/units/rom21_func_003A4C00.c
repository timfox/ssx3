#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A4C00(void);
PS2_RETAIL_ASM_ONLY(
    func_003A4C00,
    ".word 0x03e00008\n\t"
    ".word 0x8c820040\n\t"
)
#endif
