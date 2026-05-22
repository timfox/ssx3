#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
#include <cstring>
extern "C" {
#endif

extern void* cBucketMan_first(void* bucket_man, int bucket_id);
extern void* cBucketMan_next(void* bucket_man, void* node, int bucket_id);
extern void cMoveNode_addModifierBlock(void* self, void* param);
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void* tModifierBlock_tModifierBlock(void* block, void* param);

extern char D_0049E9D8[];
extern char D_0049E940[];

typedef void (*NodeConfigBindFn)(void* src, void* dst, int count);

// Retail @ 0x00355420 — bind effect-modifier config through an existing modifier block.
INCLUDE_ASM("mem/rom21_func_00355420", func_00355420);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00355420,
    ".word 0x27bdfff0\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8c83001c\n\t"
    ".word 0x1060000b\n\t"
    ".word 0x24020001\n\t"
    ".word 0x8c650000\n\t"
    ".word 0x10a00009\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x8ca30000\n\t"
    ".word 0x84640040\n\t"
    ".word 0x8c620044\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x10000002\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
)
#else
int func_00355420(void* self, void* param) {
    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        return 1;
    }
    void* cfg_root = *(void**)block;
    if (cfg_root == 0) {
        return 1;
    }
    char* iface = *(char**)cfg_root;
    const short src_offset = *(short*)((char*)iface + 0x40);
    NodeConfigBindFn bind = *(NodeConfigBindFn*)((char*)iface + 0x44);
    bind((char*)cfg_root + src_offset, cfg_root, (int)(unsigned long)param);
    return 0;
}
#endif
#endif


// Retail @ 0x00355028 — walk bucket nodes and refresh vtable/config hooks.
INCLUDE_ASM("mem/rom21_func_00355028", func_00355028);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00355028,
    ".word 0x27bdffc0\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d53aa\n\t"
    ".word 0x0080902d\n\t"
    ".word 0x0040802d\n\t"
    ".word 0x5200000e\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x8e02000c\n\t"
    ".word 0x84440018\n\t"
    ".word 0x8c43001c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0240202d\n\t"
    ".word 0x0c0d53be\n\t"
    ".word 0x0220302d\n\t"
    ".word 0x0040802d\n\t"
    ".word 0x5600fff6\n\t"
    ".word 0x8e02000c\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
    ".word 0x00000000\n\t"
)
#else
void func_00355028(void* self, int bucket_id) {
    void* node = cBucketMan_first(self, bucket_id);
    if (node == 0) {
        return;
    }
    for (;;) {
        char* iface = *(char**)((char*)node + 0xC);
        const short src_offset = *(unsigned short*)((char*)iface + 0x18);
        NodeConfigBindFn bind = *(NodeConfigBindFn*)((char*)iface + 0x1C);
        bind((char*)iface + src_offset, node, 4);

        void* link = cBucketMan_next(self, node, bucket_id);
        if (link == 0) {
            return;
        }
        node = link;
    }
}
#endif
#endif


extern void func_0035F6E8(void* spline, void* config);

// Retail @ 0x00355550 — store spline on modifier block (alloc block if needed).
INCLUDE_ASM("mem/rom21_func_00355550", func_00355550);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00355550,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8e04001c\n\t"
    ".word 0x14800005\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x0c0d551a\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x10000004\n\t"
    ".word 0x8e02001c\n\t"
    ".word 0x0c0d4c46\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8e02001c\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0xac510004\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#else
void func_00355550(void* self, void* spline) {
    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        cMoveNode_addModifierBlock(self, spline);
        block = *(void**)((char*)self + 0x1C);
    }
    if (block != 0) {
        *(void**)block = spline;
    }
}
#endif
#endif


// Retail @ 0x00355B90 — allocate spline object and run retail init chain.
INCLUDE_ASM("mem/rom21_func_00355B90", func_00355B90);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00355B90,
    ".word 0x27bdffd0\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x00a0802d\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x3c050049\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040050\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24a5e940\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0c0d7dba\n\t"
    ".word 0x0040202d\n\t"
    ".word 0x0220202d\n\t"
    ".word 0x0c0d5554\n\t"
    ".word 0x0040282d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#else
void* func_00355B90(void* spline, void* config) {
    void* mem = cMemMan_alloc(0x50UL, (void*)D_0049E940, 0x20000000UL, (void*)0);
    if (mem == 0) {
        return spline;
    }
    func_0035F6E8(mem, config);
    func_00355550(mem, config);
    return mem;
}
#endif
#endif


// Retail @ 0x003530D0 — invoke spline iface hook and clear spline root word.
INCLUDE_ASM("mem/rom21_func_003530D0", func_003530D0);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
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
#else
void func_003530D0(void* self, void* spline) {
    (void)self;
    if (spline == 0) {
        return;
    }
    void* obj = *(void**)spline;
    if (obj == 0) {
        return;
    }
    typedef void (*SplineIfaceFn)(void*, short);
    const short iface_arg = *(short*)((char*)obj + 8);
    SplineIfaceFn hook = *(SplineIfaceFn*)((char*)obj + 0xC);
    if (hook != 0) {
        hook(obj, iface_arg);
    }
    *(void**)spline = 0;
}
#endif
#endif


// Retail @ 0x003554B0 — attach spline to move node and update object flags.
INCLUDE_ASM("mem/rom21_func_003554B0", func_003554B0);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_003554B0,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x8e02000c\n\t"
    ".word 0x84440188\n\t"
    ".word 0x8c43018c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x8e04001c\n\t"
    ".word 0x14800005\n\t"
    ".word 0x00000000\n\t"
    ".word 0x0c0d551a\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x10000004\n\t"
    ".word 0x8e02001c\n\t"
    ".word 0x0c0d4c34\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8e02001c\n\t"
    ".word 0xac510000\n\t"
    ".word 0x8e03000c\n\t"
    ".word 0x84640180\n\t"
    ".word 0x8c620184\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x8e040018\n\t"
    ".word 0x3c03ffff\n\t"
    ".word 0x3463ffdf\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x8c820008\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x00431024\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x34420040\n\t"
    ".word 0xac820008\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#else
void func_003554B0(void* self, void* spline) {
    char* iface = *(char**)((char*)self + 0xC);
    const short src_offset = *(short*)((char*)iface + 0x188);
    NodeConfigBindFn bind = *(NodeConfigBindFn*)((char*)iface + 0x18C);
    bind((char*)iface + src_offset, self, 4);

    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        cMoveNode_addModifierBlock(self, spline);
    } else {
        func_003530D0(self, spline);
    }
    block = *(void**)((char*)self + 0x1C);
    if (block != 0) {
        *(void**)block = spline;
    }

    const short src_offset2 = *(short*)((char*)iface + 0x180);
    NodeConfigBindFn bind2 = *(NodeConfigBindFn*)((char*)iface + 0x184);
    bind2((char*)iface + src_offset2, self, 4);

    int flags = *(int*)((char*)self + 8);
    flags = (flags & 0xFFFFDF) | 0x40;
    *(int*)((char*)self + 8) = flags;
}
#endif
#endif


// Retail @ 0x00345798 — 16-byte scratch header at particle+0x60.
INCLUDE_ASM("mem/rom21_func_00345798", func_00345798);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00345798,
    ".word 0x8f830f04\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x244212f8\n\t"
    ".word 0xac85000c\n\t"
    ".word 0xac820008\n\t"
    ".word 0x24630001\n\t"
    ".word 0xac800000\n\t"
    ".word 0x0080102d\n\t"
    ".word 0xac800004\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xaf830f04\n\t"
    ".word 0x00000000\n\t")
#else
#ifdef SSX3_HOST
static unsigned char g_D_004912F8[0x10];
void* D_004912F8 = g_D_004912F8;
static int g_gp_0F04_particle_init;
#else
extern void* D_004912F8;
#endif
void func_00345798(void* scratch, void* param) {
    if (scratch == 0) {
        return;
    }
    char* s = static_cast<char*>(scratch);
    *(void**)(s + 0x00) = 0;
    *(void**)(s + 0x04) = 0;
    *(void**)(s + 0x08) = D_004912F8;
    *(void**)(s + 0x0C) = param;
#ifdef SSX3_HOST
    g_gp_0F04_particle_init++;
#endif
}
#endif
#endif

// Retail @ 0x00345EF8 — particle transform via GTE (particle+0x20 / +0x30 / +0x40).
INCLUDE_ASM("mem/rom21_func_00345EF8", func_00345EF8);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00345EF8,
    ".word 0x27bdff80\n\t"
    ".word 0x7fb00070\n\t"
    ".word 0x03a0302d\n\t"
    ".word 0xffbf0060\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x8e04000c\n\t"
    ".word 0x0c0d3fb6\n\t"
    ".word 0x8e050010\n\t"
    ".word 0xda080020\n\t"
    ".word 0xdba40000\n\t"
    ".word 0xdba50010\n\t"
    ".word 0xdba60020\n\t"
    ".word 0xdba70030\n\t"
    ".word 0x4be821bc\n\t"
    ".word 0x4be828bd\n\t"
    ".word 0x4be830be\n\t"
    ".word 0x4be83b0b\n\t"
    ".word 0xfbac0050\n\t"
    ".word 0xdfbf0060\n\t"
    ".word 0x7ba20050\n\t"
    ".word 0x7e020030\n\t"
    ".word 0x7fa20040\n\t"
    ".word 0xda080050\n\t"
    ".word 0xdba40000\n\t"
    ".word 0xdba50010\n\t"
    ".word 0xdba60020\n\t"
    ".word 0xdba70030\n\t"
    ".word 0x4be821bc\n\t"
    ".word 0x4be828bd\n\t"
    ".word 0x4be830be\n\t"
    ".word 0x4be83b0b\n\t"
    ".word 0xfbac0050\n\t"
    ".word 0x7ba20050\n\t"
    ".word 0x7e020040\n\t"
    ".word 0x7bb00070\n\t"
    ".word 0x7fa20040\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0080\n\t")
#else
extern void func_0034FED8(void* a0, void* a1);
void func_00345EF8(void* particle) {
    if (particle == 0) {
        return;
    }
    char* p = static_cast<char*>(particle);
    void* parent = *(void**)(p + 0x0C);
    func_0034FED8(parent, *(void**)(p + 0x10));
    /* GTE matrix at +0x20/+0x30/+0x40 not modeled on host. */
}
#endif
#endif

// Retail @ 0x00370B60 — particle+0x60 extended init (vtable hook, flag words).
INCLUDE_ASM("mem/rom21_func_00370B60", func_00370B60);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00370B60,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24423118\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0xae00000c\n\t"
    ".word 0x26040020\n\t"
    ".word 0xae000010\n\t"
    ".word 0xae000014\n\t"
    ".word 0x0c0bde9a\n\t"
    ".word 0xae0201f8\n\t"
    ".word 0x2403ffff\n\t"
    ".word 0x24020001\n\t"
    ".word 0xae030178\n\t"
    ".word 0x260401e4\n\t"
    ".word 0xae020174\n\t"
    ".word 0x2406ffff\n\t"
    ".word 0xae000170\n\t"
    ".word 0x0000282d\n\t"
    ".word 0xae00017c\n\t"
    ".word 0x260301f4\n\t"
    ".word 0xae0001a0\n\t"
    ".word 0xae0001a4\n\t"
    ".word 0xae0001e0\n\t"
    ".word 0x00000000\n\t"
    ".word 0xa4660000\n\t"
    ".word 0x24a50001\n\t"
    ".word 0x2ca20002\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x1440fffa\n\t"
    ".word 0x24630002\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x0c105884\n\t"
    ".word 0x24060010\n\t"
    ".word 0x0200102d\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
    ".word 0x00000000\n\t")
#else
extern void func_002F7A68(void* sub);
extern void func_00416210(void* dst, void* a1, int size);
#ifdef SSX3_HOST
static unsigned char g_D_00493118[0x10];
void* D_00493118 = g_D_00493118;
#else
extern void* D_00493118;
#endif
void func_00370B60(void* scratch, void* type_id) {
    (void)type_id;
    if (scratch == 0) {
        return;
    }
    char* s = static_cast<char*>(scratch);
    *(unsigned*)(s + 0x0C) = 0;
    *(unsigned*)(s + 0x10) = 0;
    *(unsigned*)(s + 0x14) = 0;
    *(void**)(s + 0x1F8) = D_00493118;
    func_002F7A68(s + 0x20);
    *(int*)(s + 0x178) = -1;
    *(int*)(s + 0x174) = 1;
    *(unsigned*)(s + 0x170) = 0;
    *(unsigned*)(s + 0x17C) = 0;
    *(unsigned*)(s + 0x1A0) = 0;
    *(unsigned*)(s + 0x1A4) = 0;
    *(unsigned*)(s + 0x1E0) = 0;
    auto* tags = reinterpret_cast<short*>(s + 0x1F4);
    for (int i = 0; i < 2; i++) {
        tags[i] = -1;
    }
    func_00416210(s + 0x1E4, 0, 0x10);
}
#endif
#endif

extern void func_0036CBF8(void* out_basis, void* zero, void* param_word, void* stack);
extern void func_00370058(void* basis, void* tmpl, void* param, void* particle_sub);
extern unsigned int AIrand();
extern void func_0036CE00(void* basis, float f12, float f13, float f14);
extern void func_0036CE28(void* basis, float f12, float f13);
extern unsigned char D_004FF120[0x50];
extern unsigned char D_004FF1A0[0x50];

// Retail @ 0x00370DC8 — copy param fields into particle (floats, rand scale).
INCLUDE_ASM("mem/rom21_func_00370DC8", func_00370DC8);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00370DC8,
    ".word 0x27bdff90\n\t"
    ".word 0x7fb10040\n\t"
    ".word 0x7fb20030\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0xe7b50068\n\t"
    ".word 0x0080902d\n\t"
    ".word 0x7fb00050\n\t"
    ".word 0x46006546\n\t"
    ".word 0x7fb30020\n\t"
    ".word 0xffbf0010\n\t"
    ".word 0xe7b40060\n\t"
    ".word 0x8e2300d0\n\t"
    ".word 0xae43000c\n\t"
    ".word 0x28620002\n\t"
    ".word 0xc62000d4\n\t"
    ".word 0x14400011\n\t"
    ".word 0xe6400014\n\t"
    ".word 0x4483a000\n\t"
    ".word 0x4680a520\n\t"
    ".word 0x0c0c5dfc\n\t"
    ".word 0x00000000\n\t"
    ".word 0x3c03007f\n\t"
    ".word 0x3c053f80\n\t"
    ".word 0x3463ffff\n\t"
    ".word 0x3c013f80\n\t"
    ".word 0x44810000\n\t"
    ".word 0x00431024\n\t"
    ".word 0x00451025\n\t"
    ".word 0x44820800\n\t"
    ".word 0x46000801\n\t"
    ".word 0x4600a502\n\t"
    ".word 0x10000002\n\t"
    ".word 0xe6540010\n\t"
    ".word 0xae400010\n\t"
    ".word 0x8e2200c4\n\t"
    ".word 0x44800000\n\t"
    ".word 0xae420004\n\t"
    ".word 0x8e2300c8\n\t"
    ".word 0xae430008\n\t"
    ".word 0xc6230008\n\t"
    ".word 0x46001834\n\t"
    ".word 0x00000000\n\t"
    ".word 0x45010008\n\t"
    ".word 0x46001806\n\t"
    ".word 0xc6200014\n\t"
    ".word 0xc621001c\n\t"
    ".word 0x3c013f00\n\t"
    ".word 0x44811000\n\t"
    ".word 0x46001800\n\t"
    ".word 0x46020842\n\t"
    ".word 0x46010000\n\t"
    ".word 0xe6400000\n\t"
    ".word 0x44800000\n\t"
    ".word 0xc6220008\n\t"
    ".word 0x46020036\n\t"
    ".word 0x00000000\n\t"
    ".word 0x45010007\n\t"
    ".word 0x0000982d\n\t"
    ".word 0xc620001c\n\t"
    ".word 0x3c013f00\n\t"
    ".word 0x44810800\n\t"
    ".word 0xc6220014\n\t"
    ".word 0x46010002\n\t"
    ".word 0x46001080\n\t"
    ".word 0x3c014270\n\t"
    ".word 0x44810000\n\t"
    ".word 0x46001082\n\t"
    ".word 0x46001064\n\t"
    ".word 0x46800860\n\t"
    ".word 0x46020834\n\t"
    ".word 0x45000004\n\t"
    ".word 0x00000000\n\t"
    ".word 0x3c013f80\n\t"
    ".word 0x44810000\n\t"
    ".word 0x46000840\n\t"
    ".word 0x46000824\n\t"
    ".word 0x44030000\n\t"
    ".word 0x8e420178\n\t"
    ".word 0x1043000d\n\t"
    ".word 0x3c040050\n\t"
    ".word 0x46001064\n\t"
    ".word 0x46800860\n\t"
    ".word 0x46020834\n\t"
    ".word 0x45000004\n\t"
    ".word 0x00000000\n\t"
    ".word 0x3c013f80\n\t"
    ".word 0x44810000\n\t"
    ".word 0x46000840\n\t"
    ".word 0x46000824\n\t"
    ".word 0xe6400178\n\t"
    ".word 0x24130001\n\t"
    ".word 0x3c040050\n\t"
    ".word 0x44800000\n\t"
    ".word 0x2484f120\n\t"
    ".word 0x78820000\n\t"
    ".word 0x7e4201b0\n\t"
    ".word 0x78830000\n\t"
    ".word 0x7e4301c0\n\t"
    ".word 0x78820000\n\t"
    ".word 0x7e4201d0\n\t"
    ".word 0xc62c0008\n\t"
    ".word 0x460c0036\n\t"
    ".word 0x00000000\n\t"
    ".word 0x45010007\n\t"
    ".word 0x8e450178\n\t"
    ".word 0xc620001c\n\t"
    ".word 0x3c013f00\n\t"
    ".word 0x44810800\n\t"
    ".word 0xc6220014\n\t"
    ".word 0x46010002\n\t"
    ".word 0x46001300\n\t"
    ".word 0x26500020\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x0200202d\n\t"
    ".word 0xc6220078\n\t"
    ".word 0x03a0382d\n\t"
    ".word 0x00450018\n\t"
    ".word 0xc620007c\n\t"
    ".word 0xc6210080\n\t"
    ".word 0x8e260004\n\t"
    ".word 0xc62d0020\n\t"
    ".word 0xc62e000c\n\t"
    ".word 0x00002812\n\t"
    ".word 0xe7a20000\n\t"
    ".word 0xe7a00004\n\t"
    ".word 0xe7a10008\n\t"
    ".word 0x0c0db2fe\n\t"
    ".word 0xafa0000c\n\t"
    ".word 0x3c050050\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x24a5f1a0\n\t"
    ".word 0x0220302d\n\t"
    ".word 0x4600ab06\n\t"
    ".word 0x0c0dc016\n\t"
    ".word 0x26470180\n\t"
    ".word 0x12600010\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x8e4201f8\n\t"
    ".word 0x84440008\n\t"
    ".word 0x8c43000c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02442021\n\t"
    ".word 0x8e4201f8\n\t"
    ".word 0x84440028\n\t"
    ".word 0x8c43002c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02442021\n\t"
    ".word 0x8e4201f8\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02442021\n\t"
    ".word 0x24020001\n\t"
    ".word 0x3c030050\n\t"
    ".word 0xae420170\n\t"
    ".word 0x3c04ffbf\n\t"
    ".word 0x24671420\n\t"
    ".word 0x68e20007\n\t"
    ".word 0x6ce20000\n\t"
    ".word 0x68e5000f\n\t"
    ".word 0x6ce50008\n\t"
    ".word 0x8ce60010\n\t"
    ".word 0xb24201eb\n\t"
    ".word 0xb64201e4\n\t"
    ".word 0xb24501f3\n\t"
    ".word 0xb64501ec\n\t"
    ".word 0xae4601f4\n\t"
    ".word 0x3484ffff\n\t"
    ".word 0x3c080040\n\t"
    ".word 0x3c05ffcf\n\t"
    ".word 0x8e4201e8\n\t"
    ".word 0x34a5ffff\n\t"
    ".word 0x8e4601ec\n\t"
    ".word 0x2407fffc\n\t"
    ".word 0x00441024\n\t"
    ".word 0x3c03e000\n\t"
    ".word 0x00481025\n\t"
    ".word 0x346303ff\n\t"
    ".word 0x00451024\n\t"
    ".word 0x00c33024\n\t"
    ".word 0x00471024\n\t"
    ".word 0xae4601ec\n\t"
    ".word 0x34420002\n\t"
    ".word 0x7bb00050\n\t"
    ".word 0xae4201e8\n\t"
    ".word 0x7bb10040\n\t"
    ".word 0x7bb20030\n\t"
    ".word 0x7bb30020\n\t"
    ".word 0xdfbf0010\n\t"
    ".word 0xc7b50068\n\t"
    ".word 0xc7b40060\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0070\n\t"
    ".word 0x00000000\n\t")
#else
#ifdef SSX3_HOST
static unsigned char g_D_00501420[0x20];
static float particle_host_rand01(void) {
    unsigned int bits = AIrand() & 0x7FFFFFu;
    bits |= 0x3F800000u;
    float unit;
    std::memcpy(&unit, &bits, sizeof(unit));
    return unit - 1.0f;
}

static void particle_host_call_type_vtables(char* particle) {
    void* sub = *(void**)(particle + 0x1F8);
    if (sub == 0) {
        return;
    }
    char* subobj = static_cast<char*>(sub);
    short off = *(short*)(subobj + 0x08);
    void (*fn0)(void*, void*) = *(void (**)(void*, void*))(subobj + 0x0C);
    if (fn0 != 0) {
        fn0(particle, particle + off);
    }
    off = *(short*)(subobj + 0x28);
    fn0 = *(void (**)(void*, void*))(subobj + 0x2C);
    if (fn0 != 0) {
        fn0(particle, particle + off);
    }
    off = *(short*)(subobj + 0x10);
    fn0 = *(void (**)(void*, void*))(subobj + 0x14);
    if (fn0 != 0) {
        fn0(particle, particle + off);
    }
}
#endif

void func_00370DC8(void* particle, void* param) {
    if (particle == 0 || param == 0) {
        return;
    }
    char* p = static_cast<char*>(particle);
    char* src = static_cast<char*>(param);

    const int mode = *(int*)(src + 0xD0);
    *(int*)(p + 0x0C) = mode;

    if (mode < 2) {
        *(float*)(p + 0x14) = *(float*)(src + 0xD4);
#ifdef SSX3_HOST
        *(float*)(p + 0x10) = static_cast<float>(mode) * particle_host_rand01();
#else
        *(unsigned*)(p + 0x10) = 0;
#endif
    } else {
        *(unsigned*)(p + 0x04) = *(unsigned*)(src + 0xC4);
        *(unsigned*)(p + 0x08) = *(unsigned*)(src + 0xC8);
        *(unsigned*)(p + 0x10) = 0;
    }

    float base = 0.0f;
    const float param_08 = *(float*)(src + 0x08);
    if (param_08 < 0.0f) {
        base = param_08;
    } else {
        base = param_08 + *(float*)(src + 0x14) + *(float*)(src + 0x1C) * 0.5f;
    }
    if (param_08 <= 0.0f) {
        const float tweak = *(float*)(src + 0x1C) * 0.5f;
        float alt = *(float*)(src + 0x14) + tweak;
        alt *= 60.0f;
        if (mode >= 2) {
            int stored = *(int*)(p + 0x178);
            int as_int = static_cast<int>(alt);
            if (static_cast<float>(as_int) < alt) {
                as_int++;
            }
            if (stored != as_int) {
                float fstore = static_cast<float>(as_int);
                if (static_cast<float>(static_cast<int>(fstore)) < fstore) {
                    fstore += 1.0f;
                }
                *(float*)(p + 0x178) = fstore;
#ifdef SSX3_HOST
                particle_host_call_type_vtables(p);
#endif
            }
        }
    }
    *(float*)(p + 0x00) = base;

    std::memcpy(p + 0x1B0, D_004FF120, 16);
    std::memcpy(p + 0x1C0, D_004FF120, 16);
    std::memcpy(p + 0x1D0, D_004FF120, 16);

    alignas(16) char basis_stack[0x40];
    std::memset(basis_stack, 0, sizeof(basis_stack));
    if (src != 0) {
        *reinterpret_cast<float*>(basis_stack + 0x00) = *reinterpret_cast<float*>(src + 0x78);
        *reinterpret_cast<float*>(basis_stack + 0x04) = *reinterpret_cast<float*>(src + 0x7C);
        *reinterpret_cast<float*>(basis_stack + 0x08) = *reinterpret_cast<float*>(src + 0x80);
    }
    int zero_i = 0;
    func_0036CBF8(p + 0x20, &zero_i, static_cast<char*>(src) + 4, basis_stack);
    func_00370058(p + 0x20, D_004FF1A0, src, p + 0x180);

#ifdef SSX3_HOST
    std::memcpy(p + 0x1E4, g_D_00501420, 0x20);
    unsigned v0 = *(unsigned*)(p + 0x1E8);
    v0 = (v0 & 0xFFBFFFFFu) | 0x400000u;
    v0 &= 0xFFCFFFFFu;
    *(unsigned*)(p + 0x1E8) = v0 | 2u;
    unsigned v1 = *(unsigned*)(p + 0x1EC);
    v1 &= 0xE00003FFu;
    *(unsigned*)(p + 0x1EC) = v1;
#endif

    *(int*)(p + 0x170) = 1;
}
#endif
#endif

#ifdef SSX3_HOST
static unsigned char g_D_00491268[0x10];
static unsigned char g_D_00491370[0x10];
unsigned char D_004FF1A0[0x50];
void* D_00491268 = g_D_00491268;
void* D_00491370 = g_D_00491370;
#else
extern void* D_00491268;
extern void* D_00491370;
extern unsigned char D_004FF1A0[0x50];
#endif

// Retail @ 0x00345C90 — init 0x270-byte dynamic particle (cMoveNode_addDynamicParticle).
INCLUDE_ASM("mem/rom21_func_00345C90", func_00345C90);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00345C90,
    ".word 0x27bdff90\n\t"
    ".word 0x7fb00060\n\t"
    ".word 0x7fb10050\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x7fb30030\n\t"
    ".word 0x7fb40020\n\t"
    ".word 0x00e0982d\n\t"
    ".word 0x0100a02d\n\t"
    ".word 0x7fb20040\n\t"
    ".word 0xffbf0010\n\t"
    ".word 0x00c0282d\n\t"
    ".word 0x0c0d15e6\n\t"
    ".word 0x26120060\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x0240202d\n\t"
    ".word 0x24421268\n\t"
    ".word 0x0c0dc2d8\n\t"
    ".word 0xae020008\n\t"
    ".word 0x3c020049\n\t"
    ".word 0xae110260\n\t"
    ".word 0x24421370\n\t"
    ".word 0xae130010\n\t"
    ".word 0xae020258\n\t"
    ".word 0x0240202d\n\t"
    ".word 0xafa0000c\n\t"
    ".word 0x3c01bf80\n\t"
    ".word 0x44816000\n\t"
    ".word 0x7a820000\n\t"
    ".word 0x7e020020\n\t"
    ".word 0xc6200048\n\t"
    ".word 0xc621004c\n\t"
    ".word 0xc6220050\n\t"
    ".word 0xe7a00000\n\t"
    ".word 0xe7a10004\n\t"
    ".word 0xe7a20008\n\t"
    ".word 0xae000014\n\t"
    ".word 0x7ba20000\n\t"
    ".word 0x7e020050\n\t"
    ".word 0xae200050\n\t"
    ".word 0xae200048\n\t"
    ".word 0xae20004c\n\t"
    ".word 0x0c0dc372\n\t"
    ".word 0x8e050260\n\t"
    ".word 0x0c0d17be\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x8f830f0c\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10050\n\t"
    ".word 0x24630001\n\t"
    ".word 0x7bb20040\n\t"
    ".word 0xaf830f0c\n\t"
    ".word 0xae000018\n\t"
    ".word 0x7bb00060\n\t"
    ".word 0x7bb30030\n\t"
    ".word 0x7bb40020\n\t"
    ".word 0xdfbf0010\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0070\n\t"
    ".word 0x00000000\n\t")
#else
void func_00345C90(void* particle, void* param, void* parent_iface, void* heap_extra,
                   void* bind_data) {
    (void)heap_extra;
    if (particle == 0) {
        return;
    }
    char* p = static_cast<char*>(particle);
    func_00345798(p + 0x60, parent_iface);
    func_00370B60(p + 0x60, D_00491268);
    *(void**)(p + 0x08) = D_00491268;
    *(void**)(p + 0x260) = param;
    *(void**)(p + 0x10) = parent_iface;
    *(void**)(p + 0x258) = D_00491370;
    if (bind_data != 0) {
        for (int i = 0; i < 16; i++) {
            p[0x20 + i] = static_cast<char*>(bind_data)[i];
        }
    }
    if (param != 0) {
        char* src = static_cast<char*>(param);
        for (int i = 0; i < 12; i++) {
            p[0x50 + i] = src[0x48 + i];
        }
        *(unsigned*)(src + 0x48) = 0;
        *(unsigned*)(src + 0x4C) = 0;
        *(unsigned*)(src + 0x50) = 0;
    }
    *(unsigned*)(p + 0x14) = 0;
    func_00370DC8(particle, param);
    func_00345EF8(particle);
    *(void**)(p + 0x18) = 0;
}
#endif
#endif


#ifdef SSX3_HOST
}
#endif
