#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003A4B50(void);
PS2_RETAIL_ASM_ONLY(
    func_003A4B50,
    ".word 0x03e00008\n\t"
    ".word 0xe48c0034\n\t"
)
#endif
