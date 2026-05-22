#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* operator_new(unsigned long size);
extern int D_00491F00;
extern int D_00491E80;
extern void func_003D5330(void* bucket_man, int bucket_id);
#ifdef SKIP_ASM
void* cBucketMan_next(void* bucket_man, void* node, int bucket_id);
PS2_RETAIL_ASM_ONLY(
    cBucketMan_next,
    ".word 0x24020044\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x00c23018\n\t"
    ".word 0x8ca20004\n\t"
    ".word 0x00661821\n\t"
    ".word 0x24630014\n\t"
    ".word 0x00431826\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0003100a\n\t"
    ".word 0x00000000\n\t"
)
#endif
