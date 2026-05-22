#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cWorldMemoryMan_activateSectionMem(void);
PS2_RETAIL_ASM_ONLY(
    cWorldMemoryMan_activateSectionMem,
    ".word 0x27bdffb0\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb10030\n\t"
    ".word 0x7fb20020\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x00a0902d\n\t"
    ".word 0x7fb00040\n\t"
    ".word 0x7fb30010\n\t"
    ".word 0x00121080\n\t"
    ".word 0x00c0982d\n\t"
    ".word 0x2630003c\n\t"
    ".word 0x3c050049\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x02028021\n\t"
    ".word 0x24a54de8\n\t"
    ".word 0x24040014\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x24050014\n\t"
    ".word 0x02651018\n\t"
    ".word 0x0240302d\n\t"
    ".word 0x0c0e9dec\n\t"
    ".word 0x00512821\n\t"
    ".word 0xae020000\n\t"
    ".word 0x7bb00040\n\t"
    ".word 0x7bb10030\n\t"
    ".word 0x7bb20020\n\t"
    ".word 0x7bb30010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0050\n\t"
)
#endif
