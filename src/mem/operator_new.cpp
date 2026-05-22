#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);

/* Retail .sbss via gp+0x0d94 — default cMemMan_alloc flags. */
#ifdef SSX3_HOST
extern unsigned long g_cMemMan_default_flags;
#else
extern unsigned long g_cMemMan_default_flags;
#endif

static const unsigned long kDefaultHeapTag = 0x004A3E88u;

INCLUDE_ASM("mem/operator_new", operator_new1);
#ifdef SKIP_ASM
void* operator_new1(unsigned long size) {
    return cMemMan_alloc(size,
                       reinterpret_cast<void*>(kDefaultHeapTag),
                       g_cMemMan_default_flags,
                       0);
}
#endif

INCLUDE_ASM("mem/operator_new", operator_new2);
#ifdef SKIP_ASM
void* operator_new2(unsigned long size) {
    return operator_new1(size);
}
#endif

#ifdef SSX3_HOST
} // extern "C"
#endif
