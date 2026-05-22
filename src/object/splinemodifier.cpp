#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

void cSpline_readFromReplayFrame(void* self, void* frame);
extern int D_0049F250;
extern int D_0049F120;

typedef void (*SplineModifierBindFn)(void* src, void* dst, int count);

static void splinemodifier_bind_field(void* config, void* self, int dst_offset) {
    char* cfg = (char*)config;
    char* iface = *(char**)cfg;
    short src_offset = *(short*)(iface + 0x10);
    SplineModifierBindFn bind = *(SplineModifierBindFn*)(iface + 0x14);
    bind(cfg + src_offset, (char*)self + dst_offset, 4);
}

// Retail @ 0x003595D8
INCLUDE_ASM("mem/rom21_cSplineModifier", cSplineModifier_cSplineModifier);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cSplineModifier_cSplineModifier,
    ".word 0x27bdffc0\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x3c03ffff\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x2442f250\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x3463ffff\n\t"
    ".word 0xae020000\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0xae0300d8\n\t"
    ".word 0x26050010\n\t"
    ".word 0xae0000e0\n\t"
    ".word 0x24060050\n\t"
    ".word 0xae0000dc\n\t"
    ".word 0x261200d8\n\t"
    ".word 0xae0000e4\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x84440010\n\t"
    ".word 0x8c430014\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02242021\n\t"
    ".word 0x0240202d\n\t"
    ".word 0x0c0d155c\n\t"
    ".word 0x0220282d\n\t"
    ".word 0x3c050050\n\t"
    ".word 0x24060001\n\t"
    ".word 0x24a5f120\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x78a30000\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7e0300b0\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0x78a40000\n\t"
    ".word 0xae060058\n\t"
    ".word 0x7e0400c0\n\t"
    ".word 0xae0000d0\n\t"
    ".word 0xae0000d4\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#else
void* cSplineModifier_cSplineModifier(void* self, void* param_2, void* config) {
    *(int*)((char*)self + 0) = (int)(unsigned long)&D_0049F250;
    *(int*)((char*)self + 0xD8) = -1;
    *(int*)((char*)self + 0xE0) = 0;
    *(int*)((char*)self + 0xDC) = 0x50;
    splinemodifier_bind_field(config, self, 0x10);
    splinemodifier_bind_field(config, self, 0xD8);
    cSpline_readFromReplayFrame(self, param_2);
    *(int*)((char*)self + 0x58) = (int)(unsigned long)&D_0049F120;
    *(int*)((char*)self + 0xD0) = 0;
    *(int*)((char*)self + 0xD4) = 0;
    return self;
}
#endif
#endif


#ifdef SSX3_HOST
}
#endif
