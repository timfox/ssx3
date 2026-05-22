#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cAppMan_setNextModule(void);
PS2_RETAIL_ASM_ONLY(
    cAppMan_setNextModule,
    ".word 0x03e00008\n\t"
    ".word 0xac850004\n\t"
)
#endif
