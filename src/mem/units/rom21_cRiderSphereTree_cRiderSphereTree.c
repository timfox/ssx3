#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cRiderSphereTree_cRiderSphereTree(void);
PS2_RETAIL_ASM_ONLY(
    cRiderSphereTree_cRiderSphereTree,
    ".word 0x24020013\n\t"
    ".word 0x2403ffff\n\t"
    ".word 0x3c02ffff\n\t"
    ".word 0x2403ffff\n\t"
    ".word 0x3442ffff\n\t"
    ".word 0xac830024\n\t"
    ".word 0xac820028\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0080102d\n\t"
    ".word 0x00000000\n\t"
)
#endif
