#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A5808(void);
PS2_RETAIL_ASM_ONLY(
    func_003A5808,
    ".word 0x03e00008\n\t"
    ".word 0x8c82000c\n\t"
)
#endif
