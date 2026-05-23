#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A4850(void);
PS2_RETAIL_ASM_ONLY(
    func_003A4850,
    ".word 0x03e00008\n\t"
    ".word 0x8c820004\n\t"
)
#endif
