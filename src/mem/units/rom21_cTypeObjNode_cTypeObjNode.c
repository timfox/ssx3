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
void* cTypeObjNode_cTypeObjNode(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    cTypeObjNode_cTypeObjNode,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d51a2\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x26050010\n\t"
    ".word 0x24631e80\n\t"
    ".word 0x24060004\n\t"
    ".word 0xae03000c\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x86050010\n\t"
    ".word 0x3c040045\n\t"
    ".word 0x2484aff0\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x00052880\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x00a42821\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x8ca30000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x24630001\n\t"
    ".word 0xaca30000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
