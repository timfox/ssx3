#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cAI_setAIState(void);
PS2_RETAIL_ASM_ONLY(
    cAI_setAIState,
    ".word 0x27bdfff0\n\t"
    ".word 0x0080182d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8c660000\n\t"
    ".word 0x10a60006\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x8c620098\n\t"
    ".word 0xac660004\n\t"
    ".word 0x0c044ed2\n\t"
    ".word 0xac62009c\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
    ".word 0x00000000\n\t"
)
#endif
