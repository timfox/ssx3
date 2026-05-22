#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cWorldView_getNumSections(void);
PS2_RETAIL_ASM_ONLY(
    cWorldView_getNumSections,
    ".word 0x8c820004\n\t"
    ".word 0x10400003\n\t"
    ".word 0x00000000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x8c420008\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0000102d\n\t"
    ".word 0x00000000\n\t"
)
#endif
