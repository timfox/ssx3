#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cAI_forceAIState(void);
PS2_RETAIL_ASM_ONLY(
    cAI_forceAIState,
    ".word 0x24a3ffff\n\t"
    ".word 0x2c620007\n\t"
    ".word 0x1040002e\n\t"
    ".word 0x0080302d\n\t"
    ".word 0x3c020045\n\t"
    ".word 0x00031880\n\t"
    ".word 0x24426ca0\n\t"
    ".word 0x00621821\n\t"
    ".word 0x8c640000\n\t"
    ".word 0x00800008\n\t"
    ".word 0x00000000\n\t"
)
#endif
