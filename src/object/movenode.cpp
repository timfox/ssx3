#include "common.h"

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

// Retail @ 0x00356468
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addModifierBlock);
#ifdef SKIP_ASM
void cMoveNode_addModifierBlock(void* self, void* param) {
    void* block = cMemMan_alloc(0x28UL, (void*)D_0049E9D8, 0x20000000UL, (void*)0);
    if (block == 0) {
        return;
    }
    tModifierBlock_tModifierBlock(block, param);
    *(void**)((char*)self + 0x1C) = block;
}
#endif

// Retail @ 0x00355658 — link effect modifier on move node or modifier block list.
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addEffectModifier);
#ifdef SKIP_ASM
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

// Retail @ 0x00355B30
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addSpline);
#ifdef SKIP_ASM
void cMoveNode_addSpline(void* self, void* param) {
    void* spline = func_00355B90((void*)0, param);
    if (spline != 0) {
        func_003554B0(self, spline);
    }
}
#endif

// Retail @ 0x00355CB0
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addParticle);
#ifdef SKIP_ASM
void cMoveNode_addParticle(void* self, void* param) {
    void* particle = cMemMan_alloc(0x1F0UL, D_0049E978, 0x20000000UL, (void*)0);
    func_003458C0(particle, param);
    cMoveNode_addEffectModifier(self, particle);
}
#endif

// Retail @ 0x00355EB0
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addHalo);
#ifdef SKIP_ASM
void cMoveNode_addHalo(void* self, void* param) {
    void* halo = cMemMan_alloc(0x40UL, D_0049E9A8, 0x20000000UL, (void*)0);
    func_00346120(halo, param);
    cMoveNode_addEffectModifier(self, halo);
}
#endif

// Retail @ 0x00355F88 — dynamic particle variant.
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_addDynamicParticle);
#ifdef SKIP_ASM
extern void func_003C5F94(void* fmt, void* a0);
extern void func_00351724(void* particle, void* param, void* heap, void* tag);

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
    func_00351724(particle, param, (void*)0, parent);
    cMoveNode_addEffectModifier(self, particle);
}
#endif

// Retail @ 0x00356088 — overlap collision system hookup.
INCLUDE_ASM("mem/rom21_cMoveNode", cMoveNode_setupOverlapSystem);
#ifdef SKIP_ASM
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

#ifdef SSX3_HOST
}
#endif
