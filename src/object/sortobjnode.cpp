#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

extern void* cObjNode_cObjNode(void* self, void* param_2, void* config);
extern int D_00491E00;

typedef void (*SortObjConfigBindFn)(void* src, void* dst, int count);

static void sortobjnode_bind_config_field(void* config, void* self, int dst_offset) {
    char* cfg = (char*)config;
    char* iface = *(char**)cfg;
    short src_offset = *(short*)(iface + 0x10);
    SortObjConfigBindFn bind = *(SortObjConfigBindFn*)(iface + 0x14);
    bind(cfg + src_offset, (char*)self + dst_offset, 4);
}

// Retail @ 0x003548C0
INCLUDE_ASM("mem/rom21_cSortObjNode", cSortObjNode_cSortObjNode);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cSortObjNode_cSortObjNode,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x0080802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d51e2\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x26050014\n\t"
    ".word 0x24631e00\n\t"
    ".word 0x24060004\n\t"
    ".word 0xae03000c\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
    ".word 0x00000000\n\t"
)
#else
void* cSortObjNode_cSortObjNode(void* self, void* param_2, void* config) {
    cObjNode_cObjNode(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_00491E00;
    sortobjnode_bind_config_field(config, self, 0x14);
    return self;
}
#endif
#endif


#ifdef SSX3_HOST
}
#endif
