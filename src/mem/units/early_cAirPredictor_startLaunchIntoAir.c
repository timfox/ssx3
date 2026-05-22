#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cAirPredictor_startLaunchIntoAir(void);
PS2_RETAIL_ASM_ONLY(
    cAirPredictor_startLaunchIntoAir,
    ".word 0x27bdffb0\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0xe7b40040\n\t"
    ".word 0x00c0902d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c044c66\n\t"
    ".word 0x46006506\n\t"
    ".word 0xe61400a8\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x0c044d86\n\t"
    ".word 0x0240302d\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0xc7b40040\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0050\n\t"
    ".word 0x00000000\n\t"
)
#endif
