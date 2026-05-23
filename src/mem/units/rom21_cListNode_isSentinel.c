#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cListNode_isSentinel(void);
PS2_RETAIL_ASM_ONLY(
    cListNode_isSentinel,
    ".word 0x8c820004\n\t"
    ".word 0x10440004\n\t"
    ".word 0x0000182d\n\t"
    ".word 0x8c820000\n\t"
    ".word 0x14440002\n\t"
    ".word 0x00000000\n\t"
    ".word 0x24030001\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0060102d\n\t"
    ".word 0x00000000\n\t"
)
#endif
