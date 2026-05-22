#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void SYNCTASK_init(void);
PS2_RETAIL_ASM_ONLY(
    SYNCTASK_init,
    ".word 0x27bdfff0\n\t"
    ".word 0x3c040052\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x2484ed98\n\t"
    ".word 0x0c0f9912\n\t"
    ".word 0x24060100\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
)
#endif
