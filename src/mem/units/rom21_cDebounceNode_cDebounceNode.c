#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern int D_004906F0;
extern void cMoveNode_cMoveNode(void* self, void* param_2, void* config);
#ifdef SKIP_ASM
void* cDebounceNode_cDebounceNode(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    cDebounceNode_cDebounceNode,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x00c0802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d54b6\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x2625002c\n\t"
    ".word 0x246306f0\n\t"
    ".word 0x24060004\n\t"
    ".word 0xae23000c\n\t"
    ".word 0x8e020000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x8e070000\n\t"
    ".word 0x26250030\n\t"
    ".word 0x24060004\n\t"
    ".word 0x84e40010\n\t"
    ".word 0x8ce20014\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x0220102d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
