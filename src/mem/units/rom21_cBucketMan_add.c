#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* operator_new(unsigned long size);
extern int D_00491F00;
extern int D_00491E80;
extern void func_003D5330(void* bucket_man, int bucket_id);
#ifdef SKIP_ASM
void cBucketMan_add(void* bucket_man, void* node, int bucket_id);
PS2_RETAIL_ASM_ONLY(
    cBucketMan_add,
    ".word 0x24020044\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x00c23018\n\t"
    ".word 0x00c31821\n\t"
    ".word 0x8c620000\n\t"
    ".word 0x30420001\n\t"
    ".word 0x5040000f\n\t"
    ".word 0x8c630008\n\t"
    ".word 0x8c630028\n\t"
    ".word 0xaca30004\n\t"
    ".word 0x8c820004\n\t"
    ".word 0x00c21021\n\t"
    ".word 0x8c430028\n\t"
    ".word 0xac650008\n\t"
    ".word 0x8c820004\n\t"
    ".word 0x00461021\n\t"
    ".word 0x24420024\n\t"
    ".word 0xaca20008\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x00c31821\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac650028\n\t"
    ".word 0xaca30004\n\t"
    ".word 0x8c820004\n\t"
    ".word 0x00c21021\n\t"
    ".word 0x8c430008\n\t"
    ".word 0xac650008\n\t"
    ".word 0x8c820004\n\t"
    ".word 0x00461021\n\t"
    ".word 0x24420004\n\t"
    ".word 0xaca20008\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x00c31821\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac650008\n\t"
    ".word 0x00000000\n\t"
)
#endif
