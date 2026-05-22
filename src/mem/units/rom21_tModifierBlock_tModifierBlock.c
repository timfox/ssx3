#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern int D_00491340;
extern int D_00491200;
typedef void (*ModifierConfigBindFn)(void* src, void* dst, int count);
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void* func_002D1BD8(void);
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
extern char D_0049E978[];
#ifdef SKIP_ASM
void* tModifierBlock_tModifierBlock(void* block, void* param);
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
#endif
