#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
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


#ifdef SSX3_HOST
}
#endif
