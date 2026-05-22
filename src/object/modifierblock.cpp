#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

extern int D_00491340;
extern int D_00491200;

typedef void (*ModifierConfigBindFn)(void* src, void* dst, int count);

static void modifierblock_bind_field(void* config, void* block, int iface_offset, int dst_offset) {
    void* cfg_ptr = *(void**)config;
    if (cfg_ptr == 0) {
        return;
    }
    char* iface = *(char**)block;
    short src_offset = *(short*)((char*)iface + iface_offset);
    ModifierConfigBindFn bind = *(ModifierConfigBindFn*)((char*)iface + (iface_offset + 4));
    bind((char*)cfg_ptr + src_offset, (char*)block + dst_offset, 4);
}

// Retail @ 0x00352AA8
INCLUDE_ASM("mem/rom21_tModifierBlock", tModifierBlock_tModifierBlock);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    tModifierBlock_tModifierBlock,
    ".word 0x3c020049\n\t"
    ".word 0x3c030049\n\t"
    ".word 0x24421340\n\t"
    ".word 0x24631200\n\t"
    ".word 0xac820018\n\t"
    ".word 0xac830024\n\t"
    ".word 0x0080102d\n\t"
    ".word 0xac800010\n\t"
    ".word 0xac800014\n\t"
    ".word 0xac80001c\n\t"
    ".word 0xac800020\n\t"
    ".word 0xac800000\n\t"
    ".word 0xac800004\n\t"
    ".word 0xac800008\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac80000c\n\t"
)
#else
void* tModifierBlock_tModifierBlock(void* block, void* param) {
    (void)param;
    *(int*)((char*)block + 0x18) = (int)(unsigned long)&D_00491340;
    *(int*)((char*)block + 0x24) = (int)(unsigned long)&D_00491200;
    *(int*)((char*)block + 0x10) = 0;
    *(int*)((char*)block + 0x14) = 0;
    *(int*)((char*)block + 0x1C) = 0;
    *(int*)((char*)block + 0x20) = 0;
    *(int*)((char*)block + 0) = 0;
    *(int*)((char*)block + 4) = 0;
    *(int*)((char*)block + 8) = 0;
    *(int*)((char*)block + 0xC) = 0;
    return block;
}
#endif
#endif

// Retail @ 0x00352BC0
INCLUDE_ASM("mem/rom21_tModifierBlock", tModifierBlock_setBoundBox);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    tModifierBlock_setBoundBox,
    ".word 0x27bdfff0\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8c860000\n\t"
    ".word 0x10c00007\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x8cc20000\n\t"
    ".word 0x84440068\n\t"
    ".word 0x8c43006c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x00c42021\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
    ".word 0x00000000\n\t"
)
#else
void tModifierBlock_setBoundBox(void* block, void* config) {
    modifierblock_bind_field(config, block, 0x68, 0x6C);
}
#endif

// Retail @ 0x00352C38
INCLUDE_ASM("mem/rom21_tModifierBlock", tModifierBlock_setRadius);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    tModifierBlock_setRadius,
    ".word 0x27bdfff0\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x8c850000\n\t"
    ".word 0x10a00007\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x8ca20000\n\t"
    ".word 0x84440078\n\t"
    ".word 0x8c43007c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x00a42021\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0010\n\t"
    ".word 0x00000000\n\t"
)
#else
void tModifierBlock_setRadius(void* block, void* config) {
    modifierblock_bind_field(config, block, 0x78, 0x7C);
}
#endif
#endif
#endif

// Retail @ 0x003536D8 — large replay loader (ObjDiff unit; asm on PS2).
INCLUDE_ASM("mem/rom21_tModifierBlock", tModifierBlock_readFromReplayFrame);
#if defined(SKIP_ASM) && defined(SSX3_HOST)
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void* func_002D1BD8(void);

extern char D_0049E8D0[];
extern char D_0049E8E0[];
extern char D_0049E8F0[];
extern char D_0049E908[];
extern char D_0049E918[];
extern char D_0049E928[];
extern char D_0049E940[];
extern char D_0049E958[];
extern char D_0049E968[];
extern char D_0049E978[];

extern void* cSplineModifier_cSplineModifier(void* self, void* param, void* config);
extern void func_0035A118(void* self, void* param);
extern void func_00357090(void* self, void* param);
extern void func_00356F68(void* self, void* param);
extern void func_0035DF70(void* self, void* param);
extern void func_00357358(void* self, void* param);
extern void func_00357798(void* self, void* param);
extern void func_0035F788(void* self, void* param);
extern void func_0035F1A8(void* self, void* param);
extern void func_00342808(void* self, void* param);
extern void func_003459A8(void* self, void* param);
extern void func_00345D80(void* self, void* param);
extern void func_003461C0(void* self, void* param);
extern void func_0035BA88(void* self, void* param);

static void modifierblock_store_replay_slot(void* block, int slot_index, void* object) {
    if (block == 0 || slot_index < 0) {
        return;
    }
    *(void**)((char*)block + (slot_index * 4)) = object;
}

static void modifierblock_spawn_replay_object(void* block, int type_id, void* frame_bytes) {
    void* object = 0;
    switch (type_id) {
    case 1:
        object = cMemMan_alloc(0xF0UL, (void*)D_0049E8D0, 0x20000000UL, (void*)0);
        if (object != 0) {
            cSplineModifier_cSplineModifier(object, frame_bytes, frame_bytes);
        }
        break;
    case 2:
        object = cMemMan_alloc(0x58UL, (void*)D_0049E8D0, 0x20000000UL, (void*)0);
        if (object != 0) {
            func_0035A118(object, frame_bytes);
        }
        break;
    case 3:
        object = cMemMan_alloc(0xB0UL, (void*)D_0049E8E0, 0x20000000UL, (void*)0);
        if (object != 0) {
            func_00357090(object, frame_bytes);
        }
        break;
    case 4:
        object = cMemMan_alloc(0x90UL, (void*)D_0049E8F0, 0x20000000UL, (void*)0);
        if (object != 0) {
            func_00356F68(object, frame_bytes);
        }
        break;
    case 5:
        object = cMemMan_alloc(0x2D0UL, (void*)D_0049E908, 0x20000000UL, (void*)0);
        if (object != 0) {
            func_0035DF70(object, frame_bytes);
        }
        break;
    case 6:
        object = cMemMan_alloc(0xB0UL, (void*)D_0049E918, 0x20000000UL, (void*)0);
        if (object != 0) {
            func_00357358(object, frame_bytes);
        }
        break;
    case 7:
        object = cMemMan_alloc(0x50UL, (void*)D_0049E940, 0x20000000UL, (void*)0);
        if (object != 0) {
            func_00357798(object, frame_bytes);
        }
        break;
    default:
        break;
    }

    if (object != 0) {
        modifierblock_store_replay_slot(block, type_id - 1, object);
    }
}

void tModifierBlock_readFromReplayFrame(void* block, void* frame_meta) {
    void* desc = *(void**)frame_meta;
    if (block == 0 || desc == 0) {
        return;
    }

    const unsigned short byte_off = *(unsigned short*)((char*)desc + 0x10);
    char* frame_bytes = (char*)frame_meta + byte_off;
    (void)func_002D1BD8();

    const int type_id = *(int*)(frame_bytes + 4);
    if (type_id <= 0) {
        return;
    }

    modifierblock_spawn_replay_object(block, type_id, frame_bytes);

    int remaining = *(int*)(frame_bytes + 8);
    while (remaining > 0) {
        modifierblock_spawn_replay_object(block, type_id, frame_bytes);
        remaining -= 1;
    }
}
#endif /* SKIP_ASM && SSX3_HOST */

#ifdef SSX3_HOST
}
#endif
