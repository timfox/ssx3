#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cObjectInterface_getInstanceMan(void);
PS2_RETAIL_ASM_ONLY(
    cObjectInterface_getInstanceMan,
    ".word 0x8f82f7b8\n\t"
    ".word 0x8c430084\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x8c620088\n\t"
)
#endif
