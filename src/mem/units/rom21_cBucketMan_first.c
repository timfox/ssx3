#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cBucketMan_first(void);
PS2_RETAIL_ASM_ONLY(
    cBucketMan_first,
    ".word 0x24020044\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x00a22818\n\t"
    ".word 0x00a32821\n\t"
    ".word 0x8ca20008\n\t"
    ".word 0x24a50014\n\t"
    ".word 0x00452826\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0005100a\n\t"
    ".word 0x00000000\n\t"
)
#endif
