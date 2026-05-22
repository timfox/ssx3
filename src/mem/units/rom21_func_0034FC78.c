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
extern void* cInstanceNode_cInstanceNode_impl(void* self, void* param_2, void* config);
extern void cInstanceMan_copyInstance_impl(void* dst, void* src);
extern int D_00491C80;
#ifdef SKIP_ASM
void func_0034FC78(void* self, void* param_2);
PS2_RETAIL_ASM_ONLY(
    func_0034FC78,
    ".word 0x03e00008\n\t"
    ".word 0xaca4000c\n\t"
)
#endif
