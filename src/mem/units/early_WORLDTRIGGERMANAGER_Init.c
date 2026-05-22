#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
void WORLDTRIGGERMANAGER_Init(void);
PS2_RETAIL_ASM_ONLY(
    WORLDTRIGGERMANAGER_Init,
    ".word 0x27bdfff0\n\t"
    ".word 0x8f8221e4\n\t"
    ".word 0x1440000a\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x3c050048\n\t"
    ".word 0x24040344\n\t"
    ".word 0x24a53588\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x0c0ad634\n\t"
    ".word 0x0040202d\n\t"
    ".word 0xaf8221e4\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
)
#endif
