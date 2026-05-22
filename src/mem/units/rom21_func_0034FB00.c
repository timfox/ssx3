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
void func_0034FB00(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    func_0034FB00,
    ".word 0x27bdffc0\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x0080902d\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d5214\n\t"
    ".word 0x00e0802d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0xae500018\n\t"
    ".word 0x24631c80\n\t"
    ".word 0x0240202d\n\t"
    ".word 0xae43000c\n\t"
    ".word 0x0c0d3f1e\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x8e020008\n\t"
    ".word 0x30420100\n\t"
    ".word 0x1440000b\n\t"
    ".word 0x0240102d\n\t"
    ".word 0x24020006\n\t"
    ".word 0x12220007\n\t"
    ".word 0x24020010\n\t"
    ".word 0x12220005\n\t"
    ".word 0x24020016\n\t"
    ".word 0x12220004\n\t"
    ".word 0x0240102d\n\t"
    ".word 0x0c0b46fc\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0240102d\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#endif
