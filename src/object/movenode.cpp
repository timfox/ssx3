#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

extern void* cObjNode_cObjNode(void* self, void* param_2, void* config);
extern void cBucketMan_add(void* bucket_man, void* node, void* param);
extern void* operator_new1(unsigned long size);
extern void* tModifierBlock_tModifierBlock(void* block, void* param);
void cEffectLink_add(void** list_head, void* node);
void cMoveNode_addEffectModifier(void* self, void* param);
void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern char D_0049E9E8[];
extern void func_003554B0(void* self, void* spline);
extern void* func_00355B90(void* spline, void* config);
extern char D_0049E9D8[];
extern char D_0049E978[];
extern char D_0049E940[];
extern char D_0049E9A8[];
#ifdef SSX3_HOST
static unsigned char g_D_0049E990[0x10];
void* D_0049E990 = g_D_0049E990;
#else
extern char D_0049E990[];
#endif
extern void func_003458C0(void* particle, void* param);
extern void func_00346120(void* halo, void* param);
extern void func_00345C90(void* particle, void* param, void* parent_iface, void* heap_extra, void* bind_data);
extern int D_004906F0;
#ifdef SSX3_HOST
extern void* D_2898;
#endif

typedef void (*MoveNodeConfigBindFn)(void* src, void* dst, int count);

static void movenode_bind_config_field(void* config, void* self, int dst_offset) {
    char* cfg = (char*)config;
    char* iface = *(char**)cfg;
    short src_offset = *(short*)(iface + 0x10);
    MoveNodeConfigBindFn bind = *(MoveNodeConfigBindFn*)(iface + 0x14);
    bind(cfg + src_offset, (char*)self + dst_offset, 4);
}

// Retail @ 0x003552D8
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_cMoveNode);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
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
#else
void* cMoveNode_cMoveNode(void* self, void* param_2, void* config) {
    cObjNode_cObjNode(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_004906F0;
    movenode_bind_config_field(config, self, 0x20);
    movenode_bind_config_field(config, self, 0x24);
    movenode_bind_config_field(config, self, 0x28);
    if (*(int*)self != 0) {
#ifdef SSX3_HOST
        cBucketMan_add(D_2898, self, param_2);
#else
        cBucketMan_add((void*)(int)(unsigned long)&D_2898, self, param_2);
#endif
    }
    return self;
}
#endif
#endif


// Retail @ 0x00356468
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addModifierBlock);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cMoveNode_addModifierBlock,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24a5e9d8\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24040028\n\t"
    ".word 0x0c0d4aaa\n\t"
    ".word 0x0040202d\n\t"
    ".word 0xae02001c\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t"
)
#else
void cMoveNode_addModifierBlock(void* self, void* param) {
    void* block = cMemMan_alloc(0x28UL, (void*)D_0049E9D8, 0x20000000UL, (void*)0);
    if (block == 0) {
        return;
    }
    tModifierBlock_tModifierBlock(block, param);
    *(void**)((char*)self + 0x1C) = block;
}
#endif
#endif


// Retail @ 0x00355658 — link effect modifier on move node or modifier block list.
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addEffectModifier);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cMoveNode_addEffectModifier,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8e02001c\n\t"
    ".word 0x14400003\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x0c0d551a\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8e04001c\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x0c0d15c0\n\t"
    ".word 0x24840010\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#else
void cMoveNode_addEffectModifier(void* self, void* param) {
    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        cMoveNode_addModifierBlock(self, param);
        block = *(void**)((char*)self + 0x1C);
        cEffectLink_add((void**)((char*)block + 0x10), param);
        return;
    }
    cEffectLink_add((void**)((char*)self + 0x10), param);
}
#endif
#endif


// Retail @ 0x00355B30
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addSpline);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cMoveNode_addSpline,
    ".word 0x27bdffd0\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x3c050049\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040058\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24a5e9e8\n\t"
    ".word 0x8e060018\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x0c0d67e2\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0c0d552c\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#else
void cMoveNode_addSpline(void* self, void* param) {
    void* spline = func_00355B90((void*)0, param);
    if (spline != 0) {
        func_003554B0(self, spline);
    }
}
#endif
#endif


// Retail @ 0x00355CB0
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addParticle);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
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
#else
void cMoveNode_addParticle(void* self, void* param) {
    void* particle = cMemMan_alloc(0x1F0UL, D_0049E978, 0x20000000UL, (void*)0);
    func_003458C0(particle, param);
    cMoveNode_addEffectModifier(self, particle);
}
#endif
#endif


// Retail @ 0x00355EB0
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addHalo);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cMoveNode_addHalo,
    ".word 0x27bdffd0\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x3c050049\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040040\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24a5e9a8\n\t"
    ".word 0x8e060018\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x0c0d1848\n\t"
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
#else
void cMoveNode_addHalo(void* self, void* param) {
    void* halo = cMemMan_alloc(0x40UL, D_0049E9A8, 0x20000000UL, (void*)0);
    func_00346120(halo, param);
    cMoveNode_addEffectModifier(self, halo);
}
#endif
#endif


// Retail @ 0x00355F88 — dynamic particle variant.
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addDynamicParticle);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cMoveNode_addDynamicParticle,
    ".word 0x27bdffb0\n\t"
    ".word 0x7fb00040\n\t"
    ".word 0x7fb10030\n\t"
    ".word 0x00c0802d\n\t"
    ".word 0x7fb20020\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x7fb30010\n\t"
    ".word 0x00a0902d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x06000007\n\t"
    ".word 0x00e0982d\n\t"
    ".word 0x8e220018\n\t"
    ".word 0x8c440080\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x0203182a\n\t"
    ".word 0x14600005\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x0c0c5f94\n\t"
    ".word 0x0240202d\n\t"
    ".word 0x10000010\n\t"
    ".word 0x7bb00040\n\t"
    ".word 0x24040270\n\t"
    ".word 0x24a5e990\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x8e260018\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0240282d\n\t"
    ".word 0x0260402d\n\t"
    ".word 0x0c0d1724\n\t"
    ".word 0x0200382d\n\t"
    ".word 0x0220202d\n\t"
    ".word 0x0c0d5596\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x7bb00040\n\t"
    ".word 0x7bb10030\n\t"
    ".word 0x7bb20020\n\t"
    ".word 0x7bb30010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0050\n\t"
)
#else
extern void func_003C5F94(void* fmt, void* a0);

void cMoveNode_addDynamicParticle(void* self, void* param) {
    void* iface = *(void**)((char*)self + 0x18);
    int type_id = *(int*)((char*)iface + 0x80);
    int expected = *(int*)((char*)param + 4);
    if (type_id < expected) {
        func_003C5F94((void*)0x0049E000, self);
        return;
    }
    void* particle = cMemMan_alloc(0x270UL, D_0049E990, 0x20000000UL, (void*)0);
    void* parent = *(void**)((char*)self + 0x18);
    func_00345C90(particle, param, parent, (void*)0, (void*)0);
    cMoveNode_addEffectModifier(self, particle);
}
#endif
#endif


// Retail @ 0x00356088 — overlap collision system hookup.
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_setupOverlapSystem);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cMoveNode_setupOverlapSystem,
    ".word 0x27bdffb0\n\t"
    ".word 0x7fb00040\n\t"
    ".word 0xffbf0030\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x8e02001c\n\t"
    ".word 0x50400002\n\t"
    ".word 0x0000102d\n\t"
    ".word 0x8c420000\n\t"
    ".word 0x1040000a\n\t"
    ".word 0x03a0282d\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0c0d415c\n\t"
    ".word 0x27a60020\n\t"
    ".word 0x8e04001c\n\t"
    ".word 0x0c0d4af0\n\t"
    ".word 0x03a0282d\n\t"
    ".word 0x8e04001c\n\t"
    ".word 0x0c0d4b0e\n\t"
    ".word 0xc7ac0020\n\t"
    ".word 0x7bb00040\n\t"
    ".word 0xdfbf0030\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0050\n\t"
    ".word 0x00000000\n\t"
)
#else
extern void func_003D415C(void* zero, void* scratch, void* overlap_out);
extern void func_003D4AF0(void* block, void* scratch);
extern void func_003D4B0E(void* block, void* overlap_out);

void cMoveNode_setupOverlapSystem(void* self) {
    void* block = *(void**)((char*)self + 0x1C);
    if (block != 0 && *(void**)block != 0) {
        return;
    }
    char scratch[0x30];
    func_003D415C(0, scratch, scratch + 0x20);
    block = *(void**)((char*)self + 0x1C);
    func_003D4AF0(block, scratch);
    func_003D4B0E(block, scratch + 0x20);
}
#endif
#endif


#ifdef SSX3_HOST
}
#endif
