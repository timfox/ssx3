#include "platform/host_abi.h"

#include <cstdlib>

extern "C" {

void cMemBlockMan_initialize(void) {
}

void* func_00319F68(void* end_ptr) {
    (void)end_ptr;
    return nullptr;
}

void* cMemMan_allocSmall(void* size_arg) {
    (void)size_arg;
    return std::malloc(16);
}

void* cMemMan_heapAlloc(void* arena, int size, void* tag) {
    (void)arena;
    (void)tag;
    return std::malloc(static_cast<size_t>(size > 0 ? size : 1));
}

void* cMemMan_heapAllocTagged(void* arena, int size, void* tag) {
    return cMemMan_heapAlloc(arena, size, tag);
}

void cMemMan_trackAlloc(void* ptr, void* tag) {
    (void)ptr;
    (void)tag;
}

} // extern "C"
