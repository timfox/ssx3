#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cSSXApp_startGameLoad(void);
PS2_RETAIL_ASM_ONLY(
    cSSXApp_startGameLoad,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c050048\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x24a5a808\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x2404022c\n\t"
    ".word 0x0c08ba5a\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x0c0c5bbe\n\t"
    ".word 0xae020084\n\t"
    ".word 0x0c08df3e\n\t"
    ".word 0x00000000\n\t"
    ".word 0xae0200c0\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
