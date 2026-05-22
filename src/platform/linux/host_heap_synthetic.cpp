#include "platform/host_heap_synthetic.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

static_assert(offsetof(HostHeapSyntheticBlock, word_at_54) == 0x54,
              "HostHeapSyntheticBlock layout must match retail +0x54 read");

namespace {

constexpr int kMaxSyntheticBlocks = 16;

HostHeapSyntheticBlock g_synthetic_blocks[kMaxSyntheticBlocks];

HostHeapSyntheticBlock* find_by_encoded(unsigned int encoded) {
    for (int i = 0; i < kMaxSyntheticBlocks; i++) {
        if (g_synthetic_blocks[i].active && g_synthetic_blocks[i].encoded_tag == encoded) {
            return &g_synthetic_blocks[i];
        }
    }
    return nullptr;
}

} // namespace

extern "C" {

void* host_heap_synthetic_from_encoded(unsigned int encoded_tag) {
    HostHeapSyntheticBlock* existing = find_by_encoded(encoded_tag);
    if (existing != nullptr) {
        return existing;
    }

    for (int i = 0; i < kMaxSyntheticBlocks; i++) {
        if (!g_synthetic_blocks[i].active) {
            std::memset(&g_synthetic_blocks[i], 0, sizeof(HostHeapSyntheticBlock));
            g_synthetic_blocks[i].active = 1;
            g_synthetic_blocks[i].encoded_tag = encoded_tag;
            g_synthetic_blocks[i].word_at_54 = 0;
            return &g_synthetic_blocks[i];
        }
    }
    return nullptr;
}

void* host_heap_synthetic_resolve(void* query_ptr) {
    if (query_ptr == nullptr) {
        return nullptr;
    }
    for (int i = 0; i < kMaxSyntheticBlocks; i++) {
        if (g_synthetic_blocks[i].active && &g_synthetic_blocks[i] == query_ptr) {
            return query_ptr;
        }
    }
    const auto encoded = static_cast<unsigned int>(
        reinterpret_cast<uintptr_t>(query_ptr));
    return find_by_encoded(encoded);
}

} // extern "C"
