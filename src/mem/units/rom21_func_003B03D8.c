#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B03D8(void);
PS2_RETAIL_ASM_ONLY(
    func_003B03D8,
    ".word 0x03e00008\n\t"
    ".word 0xac850018\n\t"
)
#endif
