#include "common.h"
#include "ps2_match.h"
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void func_003E6574(void* dst, void* src, int byte_count);
extern void* cObjectInterface_getInstanceMan(void);
extern void cInstanceMan_copyInstance(void* dst, void* src);
extern void func_003C5F8C(void* fmt, unsigned long a0, unsigned long a1, unsigned long a2);

#ifdef SSX3_HOST
static unsigned char g_D_0049EA30[8];
static unsigned char g_D_0049EA40[8];
static unsigned char g_D_0049EA50[8];
void* D_0049EA30 = g_D_0049EA30;
void* D_0049EA40 = g_D_0049EA40;
void* D_0049EA50 = g_D_0049EA50;
#else
extern char D_0049EA30[];
extern char D_0049EA40[];
extern char D_0049EA50[];
#endif

// Retail @ 0x0035A1B8
INCLUDE_ASM("mem/rom21_cMultiSplineModifier", cMultiSplineModifier_allocNodes);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
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
#else
void cMultiSplineModifier_allocNodes(void* self) {
    unsigned count = *(unsigned*)((char*)self + 0x40);
    void* nodes = cMemMan_alloc(count, D_0049EA30, 0x20000000UL, (void*)0);
    if (nodes == 0) {
        func_003C5F8C((void*)D_0049EA30, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0UL);
    }
    *(void**)((char*)self + 0x38) = nodes;

    void* spline = cMemMan_alloc(0x60UL, D_0049EA40, 0xA0000000UL, (void*)0);
    if (spline == 0) {
        func_003C5F8C((void*)D_0049EA50, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0UL);
    }
    *(void**)((char*)self + 0x44) = spline;
}
#endif
#endif


// Retail @ 0x0035A288 — wire spline nodes from modifier config.
INCLUDE_ASM("mem/rom21_cMultiSplineModifier", cMultiSplineModifier_setupNodes);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
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
#else
void cMultiSplineModifier_setupNodes(void* self) {
    void* spline = *(void**)((char*)self + 0x44);
    void* table = *(void**)((char*)self + 0x40);
#ifdef SSX3_HOST
    std::memcpy((char*)spline + 0x78, table, 0xA0);
#else
    func_003E6574((char*)spline + 0x78, table, 0xA0);
#endif
    void* spline_obj = *(void**)spline;
    *(void**)((char*)spline_obj + 0x78) = self;

    int count = *(int*)((char*)self + 4);
    if (count < 1) {
        return;
    }

    char* spline_walk = (char*)spline_obj;
    for (int i = 1; i < count; i++) {
        (void)cObjectInterface_getInstanceMan();
        void* spline_blk = *(void**)((char*)self + 0x44);
        void* cfg = *(void**)table;
        cInstanceMan_copyInstance(spline_blk, cfg);
        *(void**)(spline_walk + 8) = cfg;
        spline_walk += 0x20;
    }
}
#endif
#endif


// Retail @ 0x0035A5F8 — overlap grid for each spline child.
INCLUDE_ASM("mem/rom21_cMultiSplineModifier", cMultiSplineModifier_setupOverlapSystem);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cMultiSplineModifier_setupOverlapSystem,
    ".word 0x27bdff20\n\t"
    ".word 0x7fb300a0\n\t"
    ".word 0x03a0282d\n\t"
    ".word 0x7fb40090\n\t"
    ".word 0x0080982d\n\t"
    ".word 0x7fb60070\n\t"
    ".word 0x26660018\n\t"
    ".word 0x7fb000d0\n\t"
    ".word 0x24140001\n\t"
    ".word 0x7fb100c0\n\t"
    ".word 0x26760004\n\t"
    ".word 0x7fb200b0\n\t"
    ".word 0x7fb50080\n\t"
    ".word 0xffbf0060\n\t"
    ".word 0x8e620040\n\t"
    ".word 0x0c0d415c\n\t"
    ".word 0x8c44000c\n\t"
    ".word 0x8e620004\n\t"
    ".word 0x0282102a\n\t"
    ".word 0x10400044\n\t"
    ".word 0x2415ffff\n\t"
    ".word 0xc6c00014\n\t"
    ".word 0x00143080\n\t"
    ".word 0x8e620044\n\t"
    ".word 0x24100060\n\t"
    ".word 0xe7a00028\n\t"
    ".word 0x02908018\n\t"
    ".word 0xe7a00020\n\t"
    ".word 0x00c21021\n\t"
    ".word 0xe7a00024\n\t"
    ".word 0x26940001\n\t"
    ".word 0xafa0002c\n\t"
    ".word 0x8c430000\n\t"
    ".word 0x8e64003c\n\t"
    ".word 0x78620040\n\t"
    ".word 0x02042021\n\t"
    ".word 0x7fa20040\n\t"
    ".word 0xdba30040\n\t"
    ".word 0xdba40020\n\t"
    ".word 0x4be4196c\n\t"
    ".word 0xfba50050\n\t"
    ".word 0x7ba20050\n\t"
    ".word 0x7fa20030\n\t"
    ".word 0x7c820030\n\t"
    ".word 0x8e630044\n\t"
    ".word 0x8e65003c\n\t"
    ".word 0x00c31821\n\t"
    ".word 0x8c640000\n\t"
    ".word 0x02052821\n\t"
    ".word 0x78820040\n\t"
    ".word 0x7fa20040\n\t"
    ".word 0xdba30040\n\t"
    ".word 0xdba40020\n\t"
    ".word 0x4be41968\n\t"
    ".word 0xfba50050\n\t"
    ".word 0x7ba20050\n\t"
    ".word 0x7fa20030\n\t"
    ".word 0x7ca20040\n\t"
    ".word 0x8e630044\n\t"
    ".word 0x00c33021\n\t"
    ".word 0x0c0b46f8\n\t"
    ".word 0x8cd20000\n\t"
    ".word 0x8e63003c\n\t"
    ".word 0x0040882d\n\t"
    ".word 0x27a40030\n\t"
    ".word 0x02038021\n\t"
    ".word 0x0c0ca3ca\n\t"
    ".word 0x26050030\n\t"
    ".word 0x8fa20034\n\t"
    ".word 0x26230050\n\t"
    ".word 0x8fa50038\n\t"
    ".word 0x0240302d\n\t"
    ".word 0x02a2102a\n\t"
    ".word 0x8fa8003c\n\t"
    ".word 0x0222180b\n\t"
    ".word 0x02a5282a\n\t"
    ".word 0x24640028\n\t"
    ".word 0x02a8402a\n\t"
    ".word 0x0065200b\n\t"
    ".word 0x27a70030\n\t"
    ".word 0x24820014\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x0c0ca308\n\t"
    ".word 0x0048200a\n\t"
    ".word 0x8e620004\n\t"
    ".word 0x0282102a\n\t"
    ".word 0x5440ffbf\n\t"
    ".word 0xc6c00014\n\t"
    ".word 0x7bb000d0\n\t"
    ".word 0x7bb100c0\n\t"
    ".word 0x7bb200b0\n\t"
    ".word 0x7bb300a0\n\t"
    ".word 0x7bb40090\n\t"
    ".word 0x7bb50080\n\t"
    ".word 0x7bb60070\n\t"
    ".word 0xdfbf0060\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd00e0\n\t"
)
#else
extern void func_003D415C(void* zero, void* scratch, void* overlap_out);
extern void func_003D4AF0(void* block, void* scratch);
extern void func_003D4B0E(void* block, void* overlap_out);

#ifdef SSX3_HOST
extern "C"
#endif
void cMultiSplineModifier_setupOverlapSystem(void* self) {
    const int count = *(int*)((char*)self + 4);
    if (count < 1) {
        return;
    }

    void* spline_pack = *(void**)((char*)self + 0x44);
    if (spline_pack == 0) {
        return;
    }

    char scratch[0x60];
    std::memset(scratch, 0, sizeof(scratch));
    func_003D415C(0, scratch, scratch + 0x20);

    char* walk = (char*)*(void**)spline_pack;
    for (int i = 0; i < count; i++) {
        void* block = *(void**)(walk + 0x3C);
        if (block != 0) {
            func_003D4AF0(block, scratch);
            func_003D4B0E(block, scratch + 0x20);
        }
        if (i + 1 >= count) {
            break;
        }
        walk += 0x20;
    }
}
#endif
#endif


#ifdef SSX3_HOST
}
#endif
