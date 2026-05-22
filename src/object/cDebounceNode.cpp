#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif
void cMoveNode_cMoveNode(void* self, void* param_2, void* config);
extern int D_004906F0;
#ifdef SSX3_HOST
}
#endif

// 100% — retail @ 0x00342D10
INCLUDE_ASM("mem/rom21_cDebounceNode", cDebounceNode_cDebounceNode);
#ifdef SKIP_ASM
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
