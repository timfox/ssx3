#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cList_addToEnd(void* list, void* node);
PS2_RETAIL_ASM_ONLY(
    cList_addToEnd,
    ".word 0x8c82000c\n\t"
    ".word 0xaca20000\n\t"
    ".word 0x8c430004\n\t"
    ".word 0xaca30004\n\t"
    ".word 0xac450004\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac85000c\n\t"
    ".word 0x00000000\n\t"
)
#endif
