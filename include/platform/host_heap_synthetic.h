#ifndef PLATFORM_HOST_HEAP_SYNTHETIC_H
#define PLATFORM_HOST_HEAP_SYNTHETIC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Stand-in for retail encoded heap tags (func_003D5290 / func_003D53B8). */
struct HostHeapSyntheticBlock {
    unsigned int encoded_tag;
    int active;
    unsigned char pad_before_54[0x54 - 8];
    int word_at_54;
};

void* host_heap_synthetic_from_encoded(unsigned int encoded_tag);
void* host_heap_synthetic_resolve(void* query_ptr);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_HEAP_SYNTHETIC_H */
