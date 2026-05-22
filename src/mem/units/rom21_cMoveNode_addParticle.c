#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* cObjNode_cObjNode(void* self, void* param_2, void* config);
extern void cBucketMan_add(void* bucket_man, void* node, void* param);
extern void* operator_new1(unsigned long size);
extern void* tModifierBlock_tModifierBlock(void* block, void* param);
extern char D_0049E9E8[];
extern void func_003554B0(void* self, void* spline);
extern void* func_00355B90(void* spline, void* config);
extern char D_0049E9D8[];
extern char D_0049E978[];
extern char D_0049E940[];
extern char D_0049E9A8[];
extern char D_0049E990[];
extern void func_003458C0(void* particle, void* param);
extern void func_00346120(void* halo, void* param);
extern void func_00345C90(void* particle, void* param, void* parent_iface, void* heap_extra, void* bind_data);
extern int D_004906F0;
typedef void (*MoveNodeConfigBindFn)(void* src, void* dst, int count);
extern int D_2898;
extern void cEffectLink_add(void** list_head, void* node);
extern void cMoveNode_addEffectModifier(void* self, void* param);
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void func_003C5F94(void* fmt, void* a0);
extern void func_003D415C(void* zero, void* scratch, void* overlap_out);
extern void func_003D4AF0(void* block, void* scratch);
extern void func_003D4B0E(void* block, void* overlap_out);
#ifdef SKIP_ASM
void cMoveNode_addParticle(void* self, void* param);
PS2_RETAIL_ASM_ONLY(
    cMoveNode_addParticle,
    ".word 0x27bdffd0\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x3c050049\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x240401f0\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24a5e978\n\t"
    ".word 0x8e060018\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x0c0d1630\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0c0d5596\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#endif
