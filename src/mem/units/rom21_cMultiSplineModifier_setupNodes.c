#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void func_003E6574(void* dst, void* src, int byte_count);
extern void* cObjectInterface_getInstanceMan(void);
extern void cInstanceMan_copyInstance(void* dst, void* src);
extern void func_003C5F8C(void* fmt, unsigned long a0, unsigned long a1, unsigned long a2);
extern char D_0049EA30[];
extern char D_0049EA40[];
extern char D_0049EA50[];
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void func_003D415C(void* zero, void* scratch, void* overlap_out);
extern void func_003D4AF0(void* block, void* scratch);
extern void func_003D4B0E(void* block, void* overlap_out);
#ifdef SKIP_ASM
void cMultiSplineModifier_setupNodes(void* self);
PS2_RETAIL_ASM_ONLY(
    cMultiSplineModifier_setupNodes,
    ".word 0x27bdffa0\n\t"
    ".word 0x240600a0\n\t"
    ".word 0x7fb10040\n\t"
    ".word 0x7fb20030\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x7fb00050\n\t"
    ".word 0x24120001\n\t"
    ".word 0x7fb30020\n\t"
    ".word 0x7fb40010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8e230044\n\t"
    ".word 0x8e250040\n\t"
    ".word 0x8c620000\n\t"
    ".word 0x8c500078\n\t"
    ".word 0x0c0f995d\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x8e220044\n\t"
    ".word 0x8c430000\n\t"
    ".word 0xac700078\n\t"
    ".word 0x8e220004\n\t"
    ".word 0x0242102a\n\t"
    ".word 0x10400020\n\t"
    ".word 0x7bb00050\n\t"
    ".word 0x3c14ffff\n\t"
    ".word 0x3c13ffff\n\t"
    ".word 0x3694ffbf\n\t"
    ".word 0x3673feff\n\t"
    ".word 0x00000000\n\t"
    ".word 0x0c0b4738\n\t"
    ".word 0x00128080\n\t"
    ".word 0x8e230044\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x8e260040\n\t"
    ".word 0x26520001\n\t"
    ".word 0x02031821\n\t"
    ".word 0x0c0d449c\n\t"
    ".word 0x8c650000\n\t"
    ".word 0x8e220044\n\t"
    ".word 0x02021021\n\t"
    ".word 0x8c440000\n\t"
    ".word 0x8c830008\n\t"
    ".word 0x00741824\n\t"
    ".word 0x34630020\n\t"
    ".word 0xac830008\n\t"
    ".word 0x8e220044\n\t"
    ".word 0x02028021\n\t"
    ".word 0x8e040000\n\t"
    ".word 0x8c820008\n\t"
    ".word 0x00531024\n\t"
    ".word 0xac820008\n\t"
    ".word 0x8e230004\n\t"
    ".word 0x0243182a\n\t"
    ".word 0x1460ffe7\n\t"
    ".word 0x7bb00050\n\t"
    ".word 0x7bb10040\n\t"
    ".word 0x7bb20030\n\t"
    ".word 0x7bb30020\n\t"
    ".word 0x7bb40010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0060\n\t"
    ".word 0x00000000\n\t"
)
#endif
