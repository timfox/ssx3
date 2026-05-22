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
void cInstanceNode_getBoundBoxInfo(void* self, void* out, unsigned long flags);
PS2_RETAIL_ASM_ONLY(
    cInstanceNode_getBoundBoxInfo,
    ".word 0x8c820018\n\t"
    ".word 0x27bdffc0\n\t"
    ".word 0x3c013f80\n\t"
    ".word 0x44811000\n\t"
    ".word 0x24a80010\n\t"
    ".word 0xc4400060\n\t"
    ".word 0x03a0482d\n\t"
    ".word 0x44801800\n\t"
    ".word 0x46001106\n\t"
    ".word 0xe7a00000\n\t"
    ".word 0x0000382d\n\t"
    ".word 0xc4400064\n\t"
    ".word 0xe7a00004\n\t"
    ".word 0xc4410068\n\t"
    ".word 0xe7a2000c\n\t"
    ".word 0xe7a10008\n\t"
    ".word 0x7ba20000\n\t"
    ".word 0x7ca20000\n\t"
    ".word 0x8c830018\n\t"
    ".word 0xc460006c\n\t"
    ".word 0xe7a00000\n\t"
    ".word 0xc4610070\n\t"
    ".word 0xe7a10004\n\t"
    ".word 0xc4600074\n\t"
    ".word 0xe7a00008\n\t"
    ".word 0x7ba20000\n\t"
    ".word 0x7ca20010\n\t"
    ".word 0x8c830018\n\t"
    ".word 0x78620040\n\t"
    ".word 0x7fa20000\n\t"
    ".word 0x30e20001\n\t"
    ".word 0x00000000\n\t"
    ".word 0x50400002\n\t"
    ".word 0xc5020000\n\t"
    ".word 0xc4a20000\n\t"
    ".word 0x30e20002\n\t"
    ".word 0x50400002\n\t"
    ".word 0xc5010004\n\t"
    ".word 0xc4a10004\n\t"
    ".word 0x30e20004\n\t"
    ".word 0x50400002\n\t"
    ".word 0xc5000008\n\t"
    ".word 0xc4a00008\n\t"
    ".word 0xe7a20010\n\t"
    ".word 0xe7a10014\n\t"
    ".word 0xe7a00018\n\t"
    ".word 0xe7a4001c\n\t"
    ".word 0xdba30010\n\t"
    ".word 0xd9240000\n\t"
    ".word 0x4be4196c\n\t"
    ".word 0xfba50030\n\t"
    ".word 0x7ba20030\n\t"
    ".word 0x7fa20020\n\t"
    ".word 0xdba30020\n\t"
    ".word 0xdba50020\n\t"
    ".word 0x4b000183\n\t"
    ".word 0x4be5192a\n\t"
    ".word 0x4b04203d\n\t"
    ".word 0x4b0430be\n\t"
    ".word 0x4b04310b\n\t"
    ".word 0x48232000\n\t"
    ".word 0x44830000\n\t"
    ".word 0x46001834\n\t"
    ".word 0x00000000\n\t"
    ".word 0x45030001\n\t"
    ".word 0x460000c6\n\t"
    ".word 0x24e70001\n\t"
    ".word 0x28e20008\n\t"
    ".word 0x5440ffdb\n\t"
    ".word 0x30e20001\n\t"
    ".word 0x46030004\n\t"
    ".word 0xe4c00000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#endif
