#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cAirPredictor_reset(void);
PS2_RETAIL_ASM_ONLY(
    cAirPredictor_reset,
    ".word 0xac8000ac\n\t"
    ".word 0x3c030050\n\t"
    ".word 0xac8000a0\n\t"
    ".word 0x2463f160\n\t"
    ".word 0xac800098\n\t"
    ".word 0x3c060050\n\t"
    ".word 0xac80009c\n\t"
    ".word 0x24c6f120\n\t"
    ".word 0xac8000a4\n\t"
    ".word 0x3c05ffff\n\t"
    ".word 0x34a5ffff\n\t"
    ".word 0x2407ffff\n\t"
    ".word 0x78620000\n\t"
    ".word 0x3c08004a\n\t"
    ".word 0x7c820020\n\t"
    ".word 0x78c30000\n\t"
    ".word 0xac870090\n\t"
    ".word 0x7c830010\n\t"
    ".word 0xac850030\n\t"
    ".word 0x25065960\n\t"
    ".word 0x68c20007\n\t"
    ".word 0x6cc20000\n\t"
    ".word 0xb082003b\n\t"
    ".word 0xb4820034\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x00000000\n\t"
)
#endif
