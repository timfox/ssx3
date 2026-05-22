#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cInputMap_loadMapFile(void);
PS2_RETAIL_ASM_ONLY(
    cInputMap_loadMapFile,
    ".word 0x27bdffc0\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x00a0802d\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x0080902d\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x3c040001\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24a5dd10\n\t"
    ".word 0x348409ec\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24060100\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0220302d\n\t"
    ".word 0x0c0c850a\n\t"
    ".word 0x0040202d\n\t"
    ".word 0xae420008\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#endif
