#include "common.h"
#include "ps2_match.h"
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif
void* cObjNode_cObjNode(void* self, void* param_2, void* config);
void* cSortObjNode_cSortObjNode(void* self, void* param_2, void* config);
void func_003E6574(void* dst, void* src, int byte_count);
extern void func_002D9C00(void* self, void* param_2, void* config);
extern int D_00491E00;
extern int D_0045288C;
extern int D_00452888;
int D_00491C80;

// Retail @ 0x00354720 — cObjNode ctor tail + instance iface fields.
INCLUDE_ASM("mem/rom21_func_00354720", func_00354720);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00354720,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d5192\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x00112400\n\t"
    ".word 0x24631e80\n\t"
    ".word 0x3c020045\n\t"
    ".word 0xa6110010\n\t"
    ".word 0x2442aff0\n\t"
    ".word 0xae03000c\n\t"
    ".word 0x00042383\n\t"
    ".word 0xa6000012\n\t"
    ".word 0x00822021\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0x8c830000\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x24630001\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0xac830000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#else
void func_00354720(void* self, void* param_2, void* config) {
    cObjNode_cObjNode(self, param_2, config);
}
#endif
#endif

// Retail @ 0x00354850 — instance bucket setup (obj ctor + type counters).
INCLUDE_ASM("mem/rom21_func_00354850", func_00354850);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_00354850,
    ".word 0x27bdffd0\n\t"
    ".word 0x7fb00020\n\t"
    ".word 0x7fb10010\n\t"
    ".word 0x00c0802d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d51c8\n\t"
    ".word 0x0080882d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x24020006\n\t"
    ".word 0x24631e00\n\t"
    ".word 0x16020006\n\t"
    ".word 0xae23000c\n\t"
    ".word 0x8f822a8c\n\t"
    ".word 0x0040182d\n\t"
    ".word 0x24420001\n\t"
    ".word 0x10000005\n\t"
    ".word 0xaf822a8c\n\t"
    ".word 0x8f822a88\n\t"
    ".word 0x0040182d\n\t"
    ".word 0x24420001\n\t"
    ".word 0xaf822a88\n\t"
    ".word 0xae230014\n\t"
    ".word 0x0220102d\n\t"
    ".word 0x7bb00020\n\t"
    ".word 0x7bb10010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0030\n\t"
)
#else
void func_00354850(void* self, void* param_2, void* config) {
    (void)config;
    func_00354720(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_00491E00;
    if (*(int*)((char*)self + 0x10) == 6) {
        D_0045288C += 1;
        D_00452888 += 1;
    }
    *(int*)((char*)self + 0x14) = (int)(unsigned long)param_2;
}
#endif
#endif

// Retail @ 0x0034FC78 — *(param_2+0x0C) = self (back-link into config).
INCLUDE_ASM("mem/rom21_func_0034FC78", func_0034FC78);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_0034FC78,
    ".word 0x03e00008\n\t"
    ".word 0xaca4000c\n\t"
)
#else
void func_0034FC78(void* self, void* param_2) {
    *(void**)((char*)param_2 + 0xC) = self;
}
#endif
#endif

void func_0034F878(void* self, void* param_2, void* config) {
    (void)config;
    func_0034FC78(self, param_2);
}

extern void func_002D1CF0(void* self, void* param_2, void* config);

extern void func_00354850(void* self, void* param_2, void* config);
extern void func_0034FC78(void* self, void* tag_word);
extern void func_002D1CF0(void* self, void* param_2, void* config);

// Retail @ 0x0034FB00 — instance config dispatch before typed ctor tail.
INCLUDE_ASM("mem/rom21_func_0034FB00", func_0034FB00);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    func_0034FB00,
    ".word 0x27bdffc0\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x00c0882d\n\t"
    ".word 0x0080902d\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d5214\n\t"
    ".word 0x00e0802d\n\t"
    ".word 0x3c030049\n\t"
    ".word 0xae500018\n\t"
    ".word 0x24631c80\n\t"
    ".word 0x0240202d\n\t"
    ".word 0xae43000c\n\t"
    ".word 0x0c0d3f1e\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x8e020008\n\t"
    ".word 0x30420100\n\t"
    ".word 0x1440000b\n\t"
    ".word 0x0240102d\n\t"
    ".word 0x24020006\n\t"
    ".word 0x12220007\n\t"
    ".word 0x24020010\n\t"
    ".word 0x12220005\n\t"
    ".word 0x24020016\n\t"
    ".word 0x12220004\n\t"
    ".word 0x0240102d\n\t"
    ".word 0x0c0b46fc\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x0240102d\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#else
void func_0034FB00(void* self, void* param_2, void* config) {
    func_00354850(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_00491C80;
    func_0034FC78(self, param_2);
    int flags = *(int*)((char*)self + 8);
    if ((flags & 0x100) == 0) {
        return;
    }
    int subtype = *(int*)((char*)self + 0x10);
    if (subtype == 6 || subtype == 0x10 || subtype == 0x16) {
        func_002D1CF0(self, param_2, config);
    }
}
#endif
#endif

typedef void* (*InstanceConfigInitFn)(void* src, void* self);

static int instancenode_init_from_config(void* config, void* self) {
    char* cfg = (char*)config;
    char* iface = *(char**)cfg;
    short src_offset = *(short*)(iface + 0x20);
    InstanceConfigInitFn init = *(InstanceConfigInitFn*)(iface + 0x24);
    return (int)(unsigned long)init(cfg + src_offset, self);
}

// Retail @ 0x0034FB90
INCLUDE_ASM("mem/rom21_cInstanceNode", cInstanceNode_cInstanceNode);
#ifdef SKIP_ASM
void* cInstanceNode_cInstanceNode_impl(void* self, void* param_2, void* config);
#ifdef SSX3_HOST
extern "C"
#endif
void* cInstanceNode_cInstanceNode(void* self, void* param_2, void* config) {
    return cInstanceNode_cInstanceNode_impl(self, param_2, config);
}
void* cInstanceNode_cInstanceNode_impl(void* self, void* param_2, void* config) {
    cSortObjNode_cSortObjNode(self, param_2, config);
    *(int*)((char*)self + 0xC) = (int)(unsigned long)&D_00491C80;
    *(int*)((char*)self + 0x18) = instancenode_init_from_config(config, self);
    return self;
}
#endif

// Retail @ 0x00351270
INCLUDE_ASM("mem/rom21_cInstanceMan_copyInstance", cInstanceMan_copyInstance);
#ifdef SKIP_ASM
void cInstanceMan_copyInstance_impl(void* dst, void* src);
#ifdef SSX3_HOST
extern "C"
#endif
void cInstanceMan_copyInstance(void* dst, void* src) {
    cInstanceMan_copyInstance_impl(dst, src);
}
void cInstanceMan_copyInstance_impl(void* dst, void* src) {
    const int saved_78 = *(int*)((char*)dst + 0x78);
#ifdef SSX3_HOST
    std::memcpy(dst, src, 0xA0);
#else
    func_003E6574(dst, src, 0xA0);
#endif
    *(int*)((char*)dst + 0x78) = saved_78;
    int flags = *(int*)((char*)dst + 8);
    *(int*)((char*)dst + 8) = flags | 0x2000;
}
#endif

// Retail @ 0x00350A88 — write bound-box fields into out buffer.
INCLUDE_ASM("mem/rom21_cInstanceNode", cInstanceNode_getBoundBoxInfo);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
void cInstanceNode_getBoundBoxInfo(void* self, void* out, unsigned long flags) {
    (void)flags;
    if (out == 0 || self == 0) {
        return;
    }
    float extent = *(float*)((char*)self + 0x18);
    if (extent <= 0.0f) {
        extent = 1.0f;
    }

    const float min_x = -extent;
    const float min_y = -extent;
    const float min_z = -extent;
    const float max_x = extent;
    const float max_y = extent;
    const float max_z = extent;

    char* buf = (char*)out;
    std::memset(buf, 0, 0x78);
    *(float*)(buf + 0x60) = min_x;
    *(float*)(buf + 0x64) = min_y;
    *(float*)(buf + 0x68) = min_z;
    *(float*)(buf + 0x6C) = max_x;
    *(float*)(buf + 0x70) = max_y;
    *(float*)(buf + 0x74) = max_z;

    static const int kCornerComponent[8] = {0, 1, 2, 3, 4, 5, 0, 1};
    const float components[6] = {min_x, min_y, min_z, max_x, max_y, max_z};
    for (int corner = 0; corner < 8; corner++) {
        *(float*)(buf + (corner * 8)) = components[kCornerComponent[corner]];
    }
}
#endif

#ifdef SSX3_HOST
}
#endif
