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
#ifdef SSX3_HOST
extern "C"
#endif
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
#ifdef SSX3_HOST
void* cBucketMan_addfirst(void* bucket_man, int bucket_id) {
    char* bucket = bucket_entry(bucket_man, bucket_id);
    unsigned tail = *(unsigned*)(bucket + 0x28);
    unsigned tag = tail ^ (unsigned)(unsigned long)(bucket + 0x34);
    return tag ? 0 : (void*)(unsigned long)tail;
}
#else
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
#endif
#endif

// Retail @ 0x00354C08
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_add);
#ifdef SKIP_ASM
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

// Retail @ 0x00355118 — drain bucket list via node vtable when flags&3 != 3.
INCLUDE_ASM("mem/rom21_cBucketMan", cBucketMan_purgeBucket);
#ifdef SKIP_ASM
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

#ifdef SSX3_HOST
}
#endif
