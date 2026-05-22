#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cMemMan_trackAlloc(void);
PS2_RETAIL_ASM_ONLY(
    cMemMan_trackAlloc,
    ".word 0x27bdfff0\n\t"
    ".word 0x00a0302d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0f9912\n\t"
    ".word 0x0000282d\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
)
#endif
