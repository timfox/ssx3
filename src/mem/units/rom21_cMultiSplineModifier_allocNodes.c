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
void cMultiSplineModifier_allocNodes(void* self);
PS2_RETAIL_ASM_ONLY(
    cMultiSplineModifier_allocNodes,
    ".word 0x27bdffc0\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x24a5ea30\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x3c062000\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x26320038\n\t"
    ".word 0x8e230040\n\t"
    ".word 0x8c620080\n\t"
    ".word 0x8c500004\n\t"
    ".word 0x0c0c5f8c\n\t"
    ".word 0x00102180\n\t"
    ".word 0x12000003\n\t"
    ".word 0x2603ffff\n\t"
    ".word 0x2404ffff\n\t"
    ".word 0x00000000\n\t"
    ".word 0xae420000\n\t"
    ".word 0x24040060\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x3c06a000\n\t"
    ".word 0x8e300004\n\t"
    ".word 0x24a5ea40\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x02040018\n\t"
    ".word 0x00002012\n\t"
    ".word 0x0c0c5f8c\n\t"
    ".word 0x2632003c\n\t"
    ".word 0x12000002\n\t"
    ".word 0x2603ffff\n\t"
    ".word 0x2404ffff\n\t"
    ".word 0xae420000\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x24a5ea50\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x8e240004\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x0c0c5f8c\n\t"
    ".word 0x00042080\n\t"
    ".word 0xae220044\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
    ".word 0x00000000\n\t"
)
#endif
