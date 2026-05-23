#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A4860(void);
PS2_RETAIL_ASM_ONLY(
    func_003A4860,
    ".word 0x03e00008\n\t"
    ".word 0xac850004\n\t"
)
#endif
