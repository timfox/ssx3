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
void* cMoveNode_cMoveNode(void* self, void* param_2, void* config);
PS2_RETAIL_ASM_ONLY(
    cMoveNode_cMoveNode,
    ".word 0x27bdffc0\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0010\n\t"
    ".word 0x0c0d3ee4\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0xae00001c\n\t"
    ".word 0x24631028\n\t"
    ".word 0x26050020\n\t"
    ".word 0xae03000c\n\t"
    ".word 0x24060004\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x8e270000\n\t"
    ".word 0x26050024\n\t"
    ".word 0x24060004\n\t"
    ".word 0x84e40010\n\t"
    ".word 0x8ce20014\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x8e230000\n\t"
    ".word 0x26050028\n\t"
    ".word 0x24060004\n\t"
    ".word 0x84640010\n\t"
    ".word 0x8c620014\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x8e270000\n\t"
    ".word 0x03a0282d\n\t"
    ".word 0x24060004\n\t"
    ".word 0x84e40010\n\t"
    ".word 0x8ce30014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x8fa20000\n\t"
    ".word 0x10400006\n\t"
    ".word 0x00000000\n\t"
    ".word 0x0c0d551a\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x8e04001c\n\t"
    ".word 0x0c0d4db6\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x0c0d59e0\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x96030012\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x34630001\n\t"
    ".word 0xdfbf0010\n\t"
    ".word 0xa6030012\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#endif
