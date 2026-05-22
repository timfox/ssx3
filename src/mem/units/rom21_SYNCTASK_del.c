#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void SYNCTASK_del(void);
PS2_RETAIL_ASM_ONLY(
    SYNCTASK_del,
    ".word 0x3c020052\n\t"
    ".word 0x0080302d\n\t"
    ".word 0x8c43ed98\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x10660009\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x2483ed98\n\t"
    ".word 0x24a50001\n\t"
    ".word 0x28a20010\n\t"
    ".word 0x1040000c\n\t"
    ".word 0x24630010\n\t"
    ".word 0x8c620000\n\t"
    ".word 0x5446fffb\n\t"
    ".word 0x24a50001\n\t"
    ".word 0x28a20010\n\t"
    ".word 0x10400006\n\t"
    ".word 0x2482ed98\n\t"
    ".word 0x00051900\n\t"
    ".word 0x00621021\n\t"
    ".word 0x8c440000\n\t"
    ".word 0x50860001\n\t"
    ".word 0xac400000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x00000000\n\t"
)
#endif
