#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* cBucketMan_first(void* bucket_man, int bucket_id);
extern void* cBucketMan_next(void* bucket_man, void* node, int bucket_id);
extern void cMoveNode_addModifierBlock(void* self, void* param);
extern void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);
extern void* tModifierBlock_tModifierBlock(void* block, void* param);
extern char D_0049E9D8[];
extern char D_0049E940[];
typedef void (*NodeConfigBindFn)(void* src, void* dst, int count);
extern void func_0035F6E8(void* spline, void* config);
extern void func_0034FED8(void* a0, void* a1);
extern void func_002F7A68(void* sub);
extern void func_00416210(void* dst, void* a1, int size);
extern void func_0036CBF8(void* out_basis, void* zero, void* param_word, void* stack);
extern void func_00370058(void* basis, void* tmpl, void* param, void* particle_sub);
extern void func_0036CE00(void* basis, float f12, float f13, float f14);
extern void func_0036CE28(void* basis, float f12, float f13);
#ifdef SKIP_ASM
void func_00345C90(void* particle, void* param, void* parent_iface, void* heap_extra,
                   void* bind_data);
PS2_RETAIL_ASM_ONLY(
    func_00345C90,
    ".word 0x27bdff90\n\t"
    ".word 0x7fb00060\n\t"
    ".word 0x7fb10050\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x7fb30030\n\t"
    ".word 0x7fb40020\n\t"
    ".word 0x00e0982d\n\t"
    ".word 0x0100a02d\n\t"
    ".word 0x7fb20040\n\t"
    ".word 0xffbf0010\n\t"
    ".word 0x00c0282d\n\t"
    ".word 0x0c0d15e6\n\t"
    ".word 0x26120060\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x0240202d\n\t"
    ".word 0x24421268\n\t"
    ".word 0x0c0dc2d8\n\t"
    ".word 0xae020008\n\t"
    ".word 0x3c020049\n\t"
    ".word 0xae110260\n\t"
    ".word 0x24421370\n\t"
    ".word 0xae130010\n\t"
    ".word 0xae020258\n\t"
    ".word 0x0240202d\n\t"
    ".word 0xafa0000c\n\t"
    ".word 0x3c01bf80\n\t"
    ".word 0x44816000\n\t"
    ".word 0x7a820000\n\t"
    ".word 0x7e020020\n\t"
    ".word 0xc6200048\n\t"
    ".word 0xc621004c\n\t"
    ".word 0xc6220050\n\t"
    ".word 0xe7a00000\n\t"
    ".word 0xe7a10004\n\t"
    ".word 0xe7a20008\n\t"
    ".word 0xae000014\n\t"
    ".word 0x7ba20000\n\t"
    ".word 0x7e020050\n\t"
    ".word 0xae200050\n\t"
    ".word 0xae200048\n\t"
    ".word 0xae20004c\n\t"
    ".word 0x0c0dc372\n\t"
    ".word 0x8e050260\n\t"
    ".word 0x0c0d17be\n\t"
    ".word 0x0200202d\n\t"
    ".word 0x8f830f0c\n\t"
    ".word 0x0200102d\n\t"
    ".word 0x7bb10050\n\t"
    ".word 0x24630001\n\t"
    ".word 0x7bb20040\n\t"
    ".word 0xaf830f0c\n\t"
    ".word 0xae000018\n\t"
    ".word 0x7bb00060\n\t"
    ".word 0x7bb30030\n\t"
    ".word 0x7bb40020\n\t"
    ".word 0xdfbf0010\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0070\n\t"
    ".word 0x00000000\n\t"
)
#endif
