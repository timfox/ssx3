#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

extern void* operator_new(unsigned long size);
extern int D_00491F00;
extern int D_00491E80;

static const int kBucketStride = 0x44;

// Retail @ 0x003549E0 — allocate bucket pool and wire list sentinels.
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_init);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cBucketMan_init,
    ".word 0x27bdffc0\n\t"
    ".word 0x24020044\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x00a0882d\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x02221018\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x3c05004a\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x24a54020\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x26120004\n\t"
    ".word 0x0c0c5f8c\n\t"
    ".word 0x24440010\n\t"
    ".word 0x24450010\n\t"
    ".word 0x2624ffff\n\t"
    ".word 0xac510000\n\t"
    ".word 0x12200013\n\t"
    ".word 0x00a0182d\n\t"
    ".word 0x3c020049\n\t"
    ".word 0x2406ffff\n\t"
    ".word 0x24421f00\n\t"
    ".word 0xac620010\n\t"
    ".word 0x2484ffff\n\t"
    ".word 0xac600008\n\t"
    ".word 0xac60000c\n\t"
    ".word 0xac620020\n\t"
    ".word 0xac600018\n\t"
    ".word 0xac60001c\n\t"
    ".word 0xac620030\n\t"
    ".word 0xac600028\n\t"
    ".word 0xac60002c\n\t"
    ".word 0xac620040\n\t"
    ".word 0xac600038\n\t"
    ".word 0xac60003c\n\t"
    ".word 0x1486fff2\n\t"
    ".word 0x24630044\n\t"
    ".word 0xae450000\n\t"
    ".word 0x0000302d\n\t"
    ".word 0x1a200026\n\t"
    ".word 0xae110000\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x8e040004\n\t"
    ".word 0x24c60001\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x24830014\n\t"
    ".word 0xac830008\n\t"
    ".word 0x8e020004\n\t"
    ".word 0x00a21021\n\t"
    ".word 0xac40000c\n\t"
    ".word 0x8e030004\n\t"
    ".word 0x00a31821\n\t"
    ".word 0xac600018\n\t"
    ".word 0x8e020004\n\t"
    ".word 0x00a21021\n\t"
    ".word 0x24440004\n\t"
    ".word 0xac44001c\n\t"
    ".word 0x8e030004\n\t"
    ".word 0x00a31821\n\t"
    ".word 0x24640034\n\t"
    ".word 0xac640028\n\t"
    ".word 0x8e020004\n\t"
    ".word 0x00a21021\n\t"
    ".word 0xac40002c\n\t"
    ".word 0x8e030004\n\t"
    ".word 0x00a31821\n\t"
    ".word 0xac600038\n\t"
    ".word 0x8e040004\n\t"
    ".word 0x00a42021\n\t"
    ".word 0x24830024\n\t"
    ".word 0xac83003c\n\t"
    ".word 0x8e020004\n\t"
    ".word 0x00a21021\n\t"
    ".word 0xac400000\n\t"
    ".word 0x8e020000\n\t"
    ".word 0x00c2102a\n\t"
    ".word 0x1440ffdd\n\t"
    ".word 0x24a50044\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#else
void cBucketMan_init(void* bucket_man, int bucket_count) {
    const unsigned long pool_bytes =
        static_cast<unsigned long>(bucket_count * kBucketStride + 16);
    void* pool = operator_new(pool_bytes);
    *(int*)bucket_man = bucket_count;
    char* buckets = static_cast<char*>(pool) + 16;
    *(char**)((char*)bucket_man + 4) = buckets;

    char* cursor = buckets;
    for (int index = bucket_count - 1; index >= 0; index--) {
        *(int*)cursor = -1;
        *(int*)(cursor + 0x10) = (int)(unsigned long)&D_00491F00;
        *(void**)(cursor + 8) = 0;
        *(void**)(cursor + 0xC) = 0;
        *(int*)(cursor + 0x20) = (int)(unsigned long)&D_00491E80;
        *(void**)(cursor + 0x18) = 0;
        *(void**)(cursor + 0x1C) = 0;
        *(int*)(cursor + 0x30) = (int)(unsigned long)&D_00491E80;
        *(void**)(cursor + 0x28) = 0;
        *(void**)(cursor + 0x2C) = 0;
        *(int*)(cursor + 0x40) = (int)(unsigned long)&D_00491E80;
        *(void**)(cursor + 0x38) = 0;
        *(void**)(cursor + 0x3C) = 0;
        cursor += kBucketStride;
    }

    for (int index = 0; index < bucket_count; index++) {
        char* bucket = buckets + (index * kBucketStride);
        char* head_sentinel = bucket + 0x14;
        char* tail_sentinel = bucket + 0x34;
        *(void**)(bucket + 8) = head_sentinel;
        *(void**)(bucket + 0xC) = 0;
        *(void**)(bucket + 0x18) = 0;
        *(void**)(bucket + 0x1C) = bucket + 4;
        *(void**)(bucket + 0x28) = tail_sentinel;
        *(void**)(bucket + 0x2C) = 0;
        *(void**)(bucket + 0x38) = 0;
        *(void**)(bucket + 0x3C) = bucket + 0x24;
        *(int*)bucket = -1;
    }
}
#endif
#endif

static char* bucket_entry(void* bucket_man, int bucket_id) {
    char* pools = *(char**)((char*)bucket_man + 4);
    return pools + (bucket_id * kBucketStride);
}

// Retail @ 0x00354EA8 — pool-relative head (xor sentinel check, movz).
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_first);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
void* cBucketMan_first(void* bucket_man, int bucket_id) {
    char* bucket = bucket_entry(bucket_man, bucket_id);
    unsigned head = *(unsigned*)(bucket + 8);
    unsigned tag = head ^ (unsigned)(unsigned long)(bucket + 0x14);
    return tag ? 0 : (void*)(unsigned long)head;
}
#else
PS2_RETAIL_ASM2(
    cBucketMan_first,
    "li $2, 0x44\n\t"
    "lw $3, 4($4)\n\t"
    "mult $5, $5, $2\n\t"
    "addu $5, $5, $3\n\t"
    "lw $2, 8($5)\n\t"
    "addiu $5, $5, 0x14\n\t"
    "xor $5, $2, $5\n\t"
    "jr $31\n\t"
    "movz $2, $0, $5\n\t"
    "nop")
#endif
#endif

// Retail @ 0x00354EF8 — next link; bucket_id in $a2 (delay slot at call sites).
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_next);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
void* cBucketMan_next(void* bucket_man, void* node, int bucket_id) {
    (void)bucket_man;
    unsigned link = *(unsigned*)((char*)node + 4);
    unsigned tag = link ^ (link + (unsigned)bucket_id + 0x14u);
    return tag ? 0 : (void*)(unsigned long)link;
}
#else
PS2_RETAIL_ASM3(
    cBucketMan_next,
    "li $2, 0x44\n\t"
    "lw $3, 4($4)\n\t"
    "mult $6, $6, $2\n\t"
    "lw $2, 4($5)\n\t"
    "addu $3, $3, $6\n\t"
    "addiu $3, $3, 0x14\n\t"
    "xor $3, $2, $3\n\t"
    "jr $31\n\t"
    "movz $2, $0, $3\n\t"
    "nop")
#endif
#endif

// Retail @ 0x00354F48 — tail list head (bucket+0x28 / sentinel+0x34).
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_addfirst);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM2(
    cBucketMan_addfirst,
    "li $2, 0x44\n\t"
    "lw $3, 4($4)\n\t"
    "mult $5, $5, $2\n\t"
    "addu $5, $5, $3\n\t"
    "lw $2, 0x28($5)\n\t"
    "addiu $5, $5, 0x34\n\t"
    "xor $5, $2, $5\n\t"
    "jr $31\n\t"
    "movz $2, $0, $5\n\t"
    "nop")
#else
void* cBucketMan_addfirst(void* bucket_man, int bucket_id) {
    char* bucket = bucket_entry(bucket_man, bucket_id);
    unsigned tail = *(unsigned*)(bucket + 0x28);
    unsigned tag = tail ^ (unsigned)(unsigned long)(bucket + 0x34);
    return tag ? 0 : (void*)(unsigned long)tail;
}
#endif
#endif

// Retail @ 0x00354C08
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_add);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
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
#else
void cBucketMan_add(void* bucket_man, void* node, int bucket_id) {
    char* bucket = bucket_entry(bucket_man, bucket_id);
    int flags = *(int*)node;
    if ((flags & 1) == 0) {
        void* tail = *(void**)(bucket + 0x28);
        *(void**)((char*)node + 4) = tail;
        if (tail != 0) {
            *(void**)((char*)tail + 4) = node;
        }
        *(void**)(bucket + 0x28) = node;
        *(int*)node = flags | 1;
        return;
    }
    void* head = *(void**)(bucket + 8);
    *(void**)((char*)node + 4) = head;
    if (head != 0) {
        *(void**)((char*)head + 4) = node;
    } else {
        *(void**)(bucket + 0x28) = node;
    }
    *(void**)(bucket + 8) = node;
}
#endif
#endif


// Retail @ 0x00355118 — drain bucket list via node vtable when flags&3 != 3.
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_purgeBucket);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
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
#else
extern void func_003D5330(void* bucket_man, int bucket_id);

void cBucketMan_purgeBucket(void* bucket_man, int bucket_id) {
    func_003D5330(bucket_man, bucket_id);
    void* node = cBucketMan_first(bucket_man, bucket_id);
    if (node == 0) {
        return;
    }
    for (;;) {
        void* link = cBucketMan_next(bucket_man, node, bucket_id);
        if (link == 0) {
            return;
        }
        const unsigned short flags = *(unsigned short*)((char*)link + 8);
        if ((flags & 3u) != 3u) {
            void (*destroy)(void*) = *(void (**)(void*))((char*)link + 0xC);
            destroy(link);
        }
        node = link;
    }
}
#endif
#endif


#ifdef SSX3_HOST
}
#endif
