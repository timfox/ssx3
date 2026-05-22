#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

/* Retail heap arena @ lui 0x50 / addiu 0xF250 → 0x0050F250 */
static void* const kMemHeapArena = reinterpret_cast<void*>(0x0050F250u);

void* cMemMan_allocSmall(void* size_arg);
void* cMemMan_heapAlloc(void* arena, int size, void* tag);
void* cMemMan_heapAllocTagged(void* arena, int size, void* tag);
void cMemMan_trackAlloc(void* ptr, void* tag);

INCLUDE_ASM("mem/cmemman_alloc_body", cMemMan_allocBody);
#if defined(SKIP_ASM) && !defined(SSX3_HOST)
void* cMemMan_allocBody(void* size_arg, int alloc_size, unsigned long flags, void* heap_state) {
    void* result;
    unsigned int size;
    int aligned;

    size = reinterpret_cast<unsigned int>(size_arg);

    if (heap_state == 0 && size_arg != 0 && size < 513u) {
        result = cMemMan_allocSmall(size_arg);
    } else {
        aligned = (alloc_size <= 15) ? 16 : alloc_size;
        if (heap_state == 0) {
            result = cMemMan_heapAlloc(kMemHeapArena, aligned, size_arg);
        } else {
            result = cMemMan_heapAllocTagged(kMemHeapArena, aligned, size_arg);
        }
    }

    if (result == 0) {
        return 0;
    }
    if ((int)flags < 0) {
        return result;
    }
    cMemMan_trackAlloc(result, size_arg);
    return result;
}
#endif /* SKIP_ASM && !SSX3_HOST */

#ifdef SSX3_HOST
} // extern "C"
#endif
