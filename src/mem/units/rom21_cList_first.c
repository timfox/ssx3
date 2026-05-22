#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void* cList_first(void* list);
PS2_RETAIL_ASM_ONLY(
    cList_first,
    ".word 0x8c820004\n\t"
    ".word 0x8c430004\n\t"
    ".word 0x00621826\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0003100a\n\t"
    ".word 0x00000000\n\t"
)
#endif
