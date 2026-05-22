#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void func_002D9C00(void* self, void* param_2, void* config);
extern int D_00491E00;
extern int D_0045288C;
extern int D_00452888;
extern void* cObjNode_cObjNode(void* self, void* param_2, void* config);
extern void* cSortObjNode_cSortObjNode(void* self, void* param_2, void* config);
extern void func_003E6574(void* dst, void* src, int byte_count);
extern void func_002D1CF0(void* self, void* param_2, void* config);
extern void func_00354850(void* self, void* param_2, void* config);
extern void func_0034FC78(void* self, void* tag_word);
extern void* cInstanceNode_cInstanceNode_impl(void* self, void* param_2, void* config);
extern void cInstanceMan_copyInstance_impl(void* dst, void* src);
extern int D_00491C80;
#ifdef SKIP_ASM
void cInstanceMan_copyInstance(void* dst, void* src);
PS2_RETAIL_ASM_ONLY(
    cInstanceMan_copyInstance,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x00a0802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x00c0282d\n\t"
    ".word 0x8e110078\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0c0f995d\n\t"
    ".word 0x240600a0\n\t"
    ".word 0x8e020008\n\t"
    ".word 0xae110078\n\t"
    ".word 0x34422000\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xae020008\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#endif
