#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void cUIEngine_loadFile(void);
PS2_RETAIL_ASM_ONLY(
    cUIEngine_loadFile,
    ".word 0x27bdff60\n\t"
    ".word 0x7fb10080\n\t"
    ".word 0x7fb00090\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x3c05004a\n\t"
    ".word 0xffbf0070\n\t"
    ".word 0x03a0202d\n\t"
    ".word 0x0220302d\n\t"
    ".word 0x0c105e0a\n\t"
    ".word 0x24a54770\n\t"
    ".word 0x03a0202d\n\t"
    ".word 0x0c0f8632\n\t"
    ".word 0x0000282d\n\t"
    ".word 0xae020004\n\t"
    ".word 0x26040050\n\t"
    ".word 0x8c450008\n\t"
    ".word 0x0c0e5c4e\n\t"
    ".word 0x00452821\n\t"
    ".word 0x8e020004\n\t"
    ".word 0x26040058\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x8c460010\n\t"
    ".word 0x0c0e604e\n\t"
    ".word 0x00463021\n\t"
    ".word 0x7bb00090\n\t"
    ".word 0x24020001\n\t"
    ".word 0x7bb10080\n\t"
    ".word 0xdfbf0070\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd00a0\n\t"
    ".word 0x00000000\n\t"
)
#endif
