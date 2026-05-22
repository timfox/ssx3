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
void func_00354720(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    func_00354720,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d5192\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x00112400\n\t"
    ".word 0x24631e80\n\t"
    ".word 0x3c020045\n\t"
    ".word 0xa6110010\n\t"
    ".word 0x2442aff0\n\t"
    ".word 0xae03000c\n\t"
    ".word 0x00042383\n\t"
    ".word 0xa6000012\n\t"
    ".word 0x00822021\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x8c830000\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x24630001\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0xac830000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
