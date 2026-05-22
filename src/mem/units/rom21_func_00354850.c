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
extern void func_0034FC78(void* self, void* tag_word);
extern void* cInstanceNode_cInstanceNode_impl(void* self, void* param_2, void* config);
extern void cInstanceMan_copyInstance_impl(void* dst, void* src);
extern int D_00491C80;
#ifdef SKIP_ASM
void func_00354850(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    func_00354850,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x00c0802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d51c8\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x24020006\n\t"
    ".word 0x24631e00\n\t"
    ".word 0x16020006\n\t"
    ".word 0xae23000c\n\t"
    ".word 0x8f822a8c\n\t"
    ".word 0x0040182d\n\t"
    ".word 0x24420001\n\t"
    ".word 0x10000005\n\t"
    ".word 0xaf822a8c\n\t"
    ".word 0x8f822a88\n\t"
    ".word 0x0040182d\n\t"
    ".word 0x24420001\n\t"
    ".word 0xaf822a88\n\t"
    ".word 0xae230014\n\t"
    ".word 0x0220102d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
