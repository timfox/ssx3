#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

void MUTEX_lock(void* ctx);
void MUTEX_unlock(void* ctx);
void* cMemMan_allocBody(void* size_arg, int alloc_size, unsigned long flags, void* heap_state);

#if defined(SKIP_ASM) && !defined(SSX3_HOST)
void* cMemMan_allocSmall(void* size_arg);
void* cMemMan_heapAlloc(void* arena, int size, void* tag);
void* cMemMan_heapAllocTagged(void* arena, int size, void* tag);
void cMemMan_trackAlloc(void* ptr, void* tag);
#endif

INCLUDE_ASM("mem/cmemman_alloc", cMemMan_alloc);

#ifdef SSX3_HOST
} // extern "C"
#endif
