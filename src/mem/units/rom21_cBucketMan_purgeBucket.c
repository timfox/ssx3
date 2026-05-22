#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

extern void* operator_new(unsigned long size);
extern int D_00491F00;
extern int D_00491E80;
extern void func_003D5330(void* bucket_man, int bucket_id);
#ifdef SKIP_ASM
void cBucketMan_purgeBucket(void* bucket_man, int bucket_id);
PS2_RETAIL_ASM_ONLY(
    cBucketMan_purgeBucket,
    ".word 0x27bdffb0\n\t"
    ".word 0x7fb20020\n\t"
    ".word 0x7fb30010\n\t"
    ".word 0x00a0902d\n\t"
    ".word 0x7fb00040\n\t"
    ".word 0x7fb10030\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0c0d53aa\n\t"
    ".word 0x0080982d\n\t"
    ".word 0x0040802d\n\t"
    ".word 0x52000012\n\t"
    ".word 0x7bb00040\n\t"
    ".word 0x0260202d\n\t"
    ".word 0x00000000\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x0c0d53be\n\t"
    ".word 0x0240302d\n\t"
    ".word 0x12000007\n\t"
    ".word 0x0040882d\n\t"
    ".word 0x8e02000c\n\t"
    ".word 0x24050003\n\t"
    ".word 0x84440008\n\t"
    ".word 0x8c43000c\n\t"
    ".word 0x0060f809\n\t"
    ".word 0x02042021\n\t"
    ".word 0x0220802d\n\t"
    ".word 0x5600fff3\n\t"
    ".word 0x0260202d\n\t"
    ".word 0x7bb00040\n\t"
    ".word 0x7bb10030\n\t"
    ".word 0x7bb20020\n\t"
    ".word 0x7bb30010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0050\n\t"
    ".word 0x00000000\n\t"
)
#endif
