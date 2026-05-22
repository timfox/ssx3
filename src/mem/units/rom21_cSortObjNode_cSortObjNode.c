#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* cObjNode_cObjNode(void* self, void* param_2, void* config);
extern int D_00491E00;
typedef void (*SortObjConfigBindFn)(void* src, void* dst, int count);
#ifdef SKIP_ASM
void* cSortObjNode_cSortObjNode(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    cSortObjNode_cSortObjNode,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d51e2\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x26050014\n\t"
    ".word 0x24631e00\n\t"
    ".word 0x24060004\n\t"
    ".word 0xae03000c\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#endif
