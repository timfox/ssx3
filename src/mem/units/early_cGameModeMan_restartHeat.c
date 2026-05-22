#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cGameModeMan_restartHeat(void);
PS2_RETAIL_ASM_ONLY(
    cGameModeMan_restartHeat,
    ".word 0x27bdfff0\n\t"
    ".word 0x3c020053\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24426668\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x00031880\n\t"
    ".word 0x00621821\n\t"
    ".word 0x8c650000\n\t"
    ".word 0x8ca20000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x3c020053\n\t"
    ".word 0x24030005\n\t"
    ".word 0x244266d0\n\t"
    ".word 0x24420014\n\t"
    ".word 0x00000000\n\t"
    ".word 0xac400000\n\t"
    ".word 0x2463ffff\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x0461fffa\n\t"
    ".word 0x2442fffc\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
)
#endif
