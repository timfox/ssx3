#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* cBucketMan_first(void* bucket_man, int bucket_id);
extern void* cBucketMan_next(void* bucket_man, void* node, int bucket_id);
extern void cMoveNode_addModifierBlock(void* self, void* param);
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void* tModifierBlock_tModifierBlock(void* block, void* param);
extern char D_0049E9D8[];
extern char D_0049E940[];
typedef void (*NodeConfigBindFn)(void* src, void* dst, int count);
extern void func_0035F6E8(void* spline, void* config);
extern void func_0034FED8(void* a0, void* a1);
extern void func_002F7A68(void* sub);
extern void func_00416210(void* dst, void* a1, int size);
extern void func_0036CBF8(void* out_basis, void* zero, void* param_word, void* stack);
extern void func_00370058(void* basis, void* tmpl, void* param, void* particle_sub);
extern void func_0036CE00(void* basis, float f12, float f13, float f14);
extern void func_0036CE28(void* basis, float f12, float f13);
#ifdef SKIP_ASM
void func_003530D0(void* self, void* spline);
PS2_RETAIL_ASM_ONLY(
    func_003530D0,
    ".word 0x27bdffe0\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x8e060000\n\t"
    ".word 0x10c00007\n\t"
    ".word 0x24050003\n\t"
    ".word 0x8cc20000\n\t"
    ".word 0x84440008\n\t"
    ".word 0x8c43000c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x00c42021\n\t"
    ".word 0xae000000\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#endif
