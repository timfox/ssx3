#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void* func_002D1BE0(void* block);
extern void func_00328F28(void* out_three_words, void* param_30);
extern void func_00328C20(void* node, int mode, void* hash_root, void* stack_indices);

#ifdef SSX3_HOST
static unsigned char g_D_0048E808[0x10];
void* D_0048E808 = g_D_0048E808;
#else
extern char D_0048E808[];
#endif

// Retail @ 0x00348B90
INCLUDE_ASM("mem/rom21_cFlexRailNode_addSpaceHash", cFlexRailNode_addSpaceHash);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cFlexRailNode_addSpaceHash,
    ".word 0x27bdffb0\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x7fb00040\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x7fb10030\n\t"
    ".word 0x7fb20020\n\t"
    ".word 0x0000382d\n\t"
    ".word 0xffbf0010\n\t"
    ".word 0x24a5e808\n\t"
    ".word 0x0c0c5f5c\n\t"
    ".word 0x24040010\n\t"
    ".word 0x24030003\n\t"
    ".word 0xac50000c\n\t"
    ".word 0xac430008\n\t"
    ".word 0x0c0b46f8\n\t"
    ".word 0xae020054\n\t"
    ".word 0x0040882d\n\t"
    ".word 0x26050030\n\t"
    ".word 0x8e120054\n\t"
    ".word 0x0c0ca3ca\n\t"
    ".word 0x03a0202d\n\t"
    ".word 0x8fa20004\n\t"
    ".word 0x2407ffff\n\t"
    ".word 0x26230050\n\t"
    ".word 0x8fa50008\n\t"
    ".word 0x00e2102a\n\t"
    ".word 0x8fa6000c\n\t"
    ".word 0x0222180b\n\t"
    ".word 0x00e5282a\n\t"
    ".word 0x24640028\n\t"
    ".word 0x00e6382a\n\t"
    ".word 0x0065200b\n\t"
    ".word 0x0240302d\n\t"
    ".word 0x24820014\n\t"
    ".word 0x24050002\n\t"
    ".word 0x0047200a\n\t"
    ".word 0x0c0ca308\n\t"
    ".word 0x03a0382d\n\t"
    ".word 0x7bb00040\n\t"
    ".word 0x7bb10030\n\t"
    ".word 0x7bb20020\n\t"
    ".word 0xdfbf0010\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0050\n\t"
    ".word 0x00000000\n\t"
)
#else
void cFlexRailNode_addSpaceHash(void* self) {
    void* hdr = cMemMan_alloc(0x10UL, (void*)D_0048E808, 0x20000000UL, (void*)0);
    *(void**)((char*)hdr + 0xC) = self;
    *(int*)((char*)hdr + 8) = 3;
    *(void**)((char*)self + 0x54) = hdr;
    void* block = func_002D1BE0(hdr);

    int indices[3];
    func_00328F28(indices, (char*)self + 0x30);

    const int limit = -1;
    void* base = block;
    if (limit < indices[0]) {
        base = block;
    } else {
        base = (char*)block + 0x50;
    }
    void* node = (char*)base + 0x28;
    if (limit < indices[1]) {
        node = base;
    }
    void* alt = (char*)node + 0x14;
    if (limit < indices[2]) {
        node = alt;
    }
    func_00328C20(node, 2, *(void**)((char*)self + 0x54), indices);
}
#endif

#ifdef SSX3_HOST
}
#endif
