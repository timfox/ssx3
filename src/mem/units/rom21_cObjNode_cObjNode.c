#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

typedef void (*NodeConfigBindFn)(void* src, void* dst, int count);
extern int D_2898;
extern void cBucketMan_add(void* bucket_man, void* node, void* param);
extern int D_00491F00;
extern int D_00491E80;
extern int D_0044AFF0;
#ifdef SKIP_ASM
extern void* cObjNode_cObjNode(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    cObjNode_cObjNode,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24421f00\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x00a0302d\n\t"
    ".word 0xae02000c\n\t"
    ".word 0x27842898\n\t"
    ".word 0x0c0d5302\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0200102d\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
)
#endif
