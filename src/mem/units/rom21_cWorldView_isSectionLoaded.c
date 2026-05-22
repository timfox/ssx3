#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cWorldView_isSectionLoaded(void);
PS2_RETAIL_ASM_ONLY(
    cWorldView_isSectionLoaded,
    ".word 0x8c820004\n\t"
    ".word 0x10400005\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8c420008\n\t"
    ".word 0x00a2102b\n\t"
    ".word 0x14400003\n\t"
    ".word 0x000518c0\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0000102d\n\t"
    ".word 0x00831821\n\t"
    ".word 0x8c620014\n\t"
    ".word 0x2442fffb\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x2c420002\n\t"
)
#endif
