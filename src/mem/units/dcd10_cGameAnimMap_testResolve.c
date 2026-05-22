#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cGameAnimMap_testResolve(void);
PS2_RETAIL_ASM_ONLY(
    cGameAnimMap_testResolve,
    ".word 0x03e00008\n\t"
    ".word 0x00000000\n\t"
)
#endif
