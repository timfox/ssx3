#include "visualfx/crowdrender2d.h"

#if defined(SSX3_HOST)

#include <stdint.h>

extern unsigned char D_00445D38[];

static const char kCrowdArchivePath[] = "data/textures/crwd.ssh";

struct CrowdGlobalConfig {
    char pad0[4];
    short anim_field_04;
    short anim_field_06;
    char pad1[5];
    char anim_field_0c;
};

struct CrowdRender2D {
    void* archive;
    int field_04;
    int field_08;
    int field_0c;
};

static void* FILE_load(const char* path) {
    (void)path;
    return reinterpret_cast<void*>(1);
}

static void* SHAPE_locate(void* archive, char* shape_name) {
    (void)archive;
    (void)shape_name;
    return 0;
}

void cCrowdRender2D_init(void) {
    static int g_crowd_initialized = 0;
    if (g_crowd_initialized) {
        return;
    }
    g_crowd_initialized = 1;

    static CrowdRender2D g_crowd_render2d_obj;
    CrowdRender2D* s3 = &g_crowd_render2d_obj;
    static const int kCrowdVariantStride = 0xd8;
    static const int kCrowdShapeSlotBias = 0x94;
    static const int kCrowdVariantCount = 16;
    static const int kCrowdTableBytes = 0x1000;
    static const int kMaxShapesPerVariant = 16;

    unsigned char* variant_root = D_00445D38 + 4;
    char* name_root = reinterpret_cast<char*>(D_00445D38 + 8);
    int* shape_slot_base = reinterpret_cast<int*>(variant_root + kCrowdShapeSlotBias);

    s3->archive = FILE_load(kCrowdArchivePath);

    for (int variant_index = 0; variant_index < kCrowdVariantCount; variant_index++) {
        const int variant_off = variant_index * kCrowdVariantStride;
        if (variant_off + kCrowdVariantStride > kCrowdTableBytes) {
            break;
        }
        char* variant = reinterpret_cast<char*>(variant_root + variant_off);
        int shape_count = *reinterpret_cast<int*>(variant);
        if (shape_count <= 0 || shape_count > kMaxShapesPerVariant) {
            continue;
        }
        int* slot = reinterpret_cast<int*>(reinterpret_cast<char*>(shape_slot_base) + variant_off);
        char* name_entry = name_root + variant_off;
        for (int shape_index = 0; shape_index < shape_count; shape_index++) {
            void* shape = SHAPE_locate(s3->archive, name_entry);
            slot[shape_index] = static_cast<int>(reinterpret_cast<uintptr_t>(shape));
            name_entry += 5;
        }
    }

    CrowdGlobalConfig* cfg = reinterpret_cast<CrowdGlobalConfig*>(D_00445D38 + 0x58);
    s3->field_04 = cfg->anim_field_04;
    s3->field_08 = cfg->anim_field_06;
    s3->field_0c = cfg->anim_field_0c;
}

#endif /* SSX3_HOST */
