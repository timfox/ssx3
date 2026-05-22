#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cEffectLink_add(void** list_head, void* node);
PS2_RETAIL_ASM_ONLY(
    cEffectLink_add,
    ".word 0x8c820000\n\t"
    ".word 0x54400001\n\t"
    ".word 0xac450004\n\t"
    ".word 0xaca40004\n\t"
    ".word 0x8c820000\n\t"
    ".word 0xaca20000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac850000\n\t"
)
#endif
