#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cList_addToFront(void* list, void* node);
PS2_RETAIL_ASM_ONLY(
    cList_addToFront,
    ".word 0x8c820004\n\t"
    ".word 0xaca20004\n\t"
    ".word 0x8c430000\n\t"
    ".word 0xaca30000\n\t"
    ".word 0xac450000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac850004\n\t"
    ".word 0x00000000\n\t"
)
#endif
