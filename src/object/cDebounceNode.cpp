#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif
void cMoveNode_cMoveNode(void* self, void* param_2, void* config);
extern int D_004906F0;

// 100% — retail @ 0x00342D10
INCLUDE_ASM("mem/rom21_cDebounceNode", cDebounceNode_cDebounceNode);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cDebounceNode_cDebounceNode,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x00c0802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d54b6\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x2625002c\n\t"
    ".word 0x246306f0\n\t"
    ".word 0x24060004\n\t"
    ".word 0xae23000c\n\t"
    ".word 0x8e020000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x8e070000\n\t"
    ".word 0x26250030\n\t"
    ".word 0x24060004\n\t"
    ".word 0x84e40010\n\t"
    ".word 0x8ce20014\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x0220102d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#else
typedef void (*DebounceBindFn)(void* src, void* dst, int count);

static void debounce_bind_field(void* config, void* self, int dst_offset) {
    char* cfg = (char*)config;
    char* iface = *(char**)cfg;
    short src_offset = *(short*)(iface + 0x10);
    DebounceBindFn bind = *(DebounceBindFn*)(iface + 0x14);
    bind(cfg + src_offset, (char*)self + dst_offset, 4);
}

#ifdef SSX3_HOST
extern "C"
#endif
void* cDebounceNode_cDebounceNode(void* self, void* param_2, void* config) {
    cMoveNode_cMoveNode(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_004906F0;
    debounce_bind_field(config, self, 0x2C);
    debounce_bind_field(config, self, 0x30);
    return self;
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif

