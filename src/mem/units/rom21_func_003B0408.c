#include "common.h"
#include "ps2_match.h"

#ifdef SKIP_ASM
void func_003B0408(void);
PS2_RETAIL_ASM_ONLY(
    func_003B0408,
    ".word 0x03e00008\n\t"
    ".word 0xa0850027\n\t"
)
#endif
