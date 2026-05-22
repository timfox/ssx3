#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cAirPredictor_initLaunch(void);
PS2_RETAIL_ASM_ONLY(
    cAirPredictor_initLaunch,
    ".word 0xac8000ac\n\t"
    ".word 0xc48000a0\n\t"
    ".word 0x78a20000\n\t"
    ".word 0x7c820070\n\t"
    ".word 0x78c30000\n\t"
    ".word 0x7c830080\n\t"
    ".word 0x78a20000\n\t"
    ".word 0x7c820050\n\t"
    ".word 0x78c30000\n\t"
    ".word 0xe4800098\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x7c830060\n\t"
)
#endif
