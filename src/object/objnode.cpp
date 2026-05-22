#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif
void cBucketMan_add(void* bucket_man, void* node, void* param);
int D_00491F00;
int D_00491E80;
int D_0044AFF0;
#ifdef SSX3_HOST
extern void* D_2898;
#endif

typedef void (*NodeConfigBindFn)(void* src, void* dst, int count);

static void objnode_bind_config_field(void* config, void* self, int dst_offset) {
    char* cfg = (char*)config;
    char* iface = *(char**)cfg;
    short src_offset = *(short*)(iface + 0x10);
    NodeConfigBindFn bind = *(NodeConfigBindFn*)(iface + 0x14);
    bind(cfg + src_offset, (char*)self + dst_offset, 4);
}

// Retail @ 0x00354688
INCLUDE_ASM("mem/rom21_cObjNode", cObjNode_cObjNode);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cObjNode_cObjNode,
    ".word 0x27bdffe0\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24421f00\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x00a0302d\n\t"
    ".word 0xae02000c\n\t"
    ".word 0x27842898\n\t"
    ".word 0x0c0d5302\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0200102d\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
)
#else
void* cObjNode_cObjNode(void* self, void* param_2, void* param_3) {
    (void)param_3;
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_00491F00;
#ifdef SSX3_HOST
    cBucketMan_add(D_2898, self, param_2);
#else
    cBucketMan_add((void*)(int)(unsigned long)&D_2898, self, param_2);
#endif
    return self;
}
#endif
#endif

// Retail @ 0x00354788
INCLUDE_ASM("mem/rom21_cTypeObjNode", cTypeObjNode_cTypeObjNode);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cTypeObjNode_cTypeObjNode,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d51a2\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x26050010\n\t"
    ".word 0x24631e80\n\t"
    ".word 0x24060004\n\t"
    ".word 0xae03000c\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x86050010\n\t"
    ".word 0x3c040045\n\t"
    ".word 0x2484aff0\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x00052880\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x00a42821\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x8ca30000\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x24630001\n\t"
    ".word 0xaca30000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#else
void* cTypeObjNode_cTypeObjNode(void* self, void* param_2, void* config) {
    cObjNode_cObjNode(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_00491E80;
    objnode_bind_config_field(config, self, 0x10);
    const int type_index = *(short*)((char*)self + 0x10);
    int* counter = (int*)((char*)&D_0044AFF0 + (type_index * 4));
    *counter = *counter + 1;
    return self;
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif

