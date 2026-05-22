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
void tModifierBlock_setBoundBox(void* block, void* config);
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
#endif
