#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cSSXApp_loadexecpurge(void);
PS2_RETAIL_ASM_ONLY(
    cSSXApp_loadexecpurge,
    ".word 0x27bdffe0\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0ad2d2\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x0c0a130a\n\t"
    ".word 0x00000000\n\t"
    ".word 0x0c0c5d00\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x8f852a90\n\t"
    ".word 0x8ca210d8\n\t"
    ".word 0x844403b8\n\t"
    ".word 0x8c4303bc\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
