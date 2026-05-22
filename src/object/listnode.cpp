#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

// Retail @ 0x00397718 — sentinel when next==self, else prev!=self.
INCLUDE_ASM("mem/rom21_cListNode", cListNode_isSentinel);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cListNode_isSentinel,
    ".word 0x8c820004\n\t"
    ".word 0x10440004\n\t"
    ".word 0x0000182d\n\t"
    ".word 0x8c820000\n\t"
    ".word 0x14440002\n\t"
    ".word 0x00000000\n\t"
    ".word 0x24030001\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0060102d\n\t"
    ".word 0x00000000\n\t"
)
#else
int cListNode_isSentinel(void* node) {
    if (*(void**)((char*)node + 4) == node) {
        return 1;
    }
    return (*(void**)node != node) ? 1 : 0;
}
#endif
#endif

// Retail @ 0x00397740
INCLUDE_ASM("mem/rom21_cListNode", cListNode_removeFromList);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cListNode_removeFromList,
    ".word 0x8c820000\n\t"
    ".word 0x10400006\n\t"
    ".word 0x8c830004\n\t"
    ".word 0x10440004\n\t"
    ".word 0x00000000\n\t"
    ".word 0x54640002\n\t"
    ".word 0xac430004\n\t"
    ".word 0xac420004\n\t"
    ".word 0x50600007\n\t"
    ".word 0xac840000\n\t"
    ".word 0x50640005\n\t"
    ".word 0xac840000\n\t"
    ".word 0x54440002\n\t"
    ".word 0xac620000\n\t"
    ".word 0xac630000\n\t"
    ".word 0xac840000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac840004\n\t"
)
#else
void cListNode_removeFromList(void* node) {
    void* prev = *(void**)node;
    void* next = *(void**)((char*)node + 4);
    if (prev != node) {
        *(void**)((char*)next + 0) = prev;
    }
    if (next != node) {
        *(void**)((char*)prev + 4) = next;
    }
    *(void**)node = node;
    *(void**)((char*)node + 4) = node;
}
#endif
#endif

// Retail @ 0x00397760 — first node or NULL when list empty.
INCLUDE_ASM("mem/rom21_cList", cList_first);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cList_first,
    ".word 0x8c820004\n\t"
    ".word 0x8c430004\n\t"
    ".word 0x00621826\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x0003100a\n\t"
    ".word 0x00000000\n\t"
)
#else
void* cList_first(void* list) {
    void* head = *(void**)((char*)list + 4);
    void* next = *(void**)((char*)head + 4);
    if (head == next) {
        return 0;
    }
    return head;
}
#endif
#endif

// Retail @ 0x00397778
INCLUDE_ASM("mem/rom21_cList", cList_addToFront);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cList_addToFront,
    ".word 0x8c820004\n\t"
    ".word 0xaca20004\n\t"
    ".word 0x8c430000\n\t"
    ".word 0xaca30000\n\t"
    ".word 0xac450000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac850004\n\t"
    ".word 0x00000000\n\t"
)
#else
void cList_addToFront(void* list, void* node) {
    void* head = *(void**)((char*)list + 4);
    *(void**)((char*)node + 4) = head;
    *(void**)((char*)node + 0) = *(void**)head;
    *(void**)((char*)head + 0) = node;
    *(void**)((char*)list + 4) = node;
}
#endif
#endif

// Retail @ 0x00397798
INCLUDE_ASM("mem/rom21_cList", cList_addToEnd);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cList_addToEnd,
    ".word 0x8c82000c\n\t"
    ".word 0xaca20000\n\t"
    ".word 0x8c430004\n\t"
    ".word 0xaca30004\n\t"
    ".word 0xac450004\n\t"
    ".word 0x03e00008\n\t"
    ".word 0xac85000c\n\t"
    ".word 0x00000000\n\t"
)
#else
void cList_addToEnd(void* list, void* node) {
    void* tail = *(void**)((char*)list + 0xC);
    *(void**)((char*)node + 0) = tail;
    *(void**)((char*)node + 4) = *(void**)((char*)tail + 4);
    *(void**)((char*)tail + 4) = node;
    *(void**)((char*)list + 0xC) = node;
}
#endif
#endif

#ifdef SSX3_HOST
}
#endif
