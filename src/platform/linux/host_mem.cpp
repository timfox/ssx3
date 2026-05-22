#include "platform/host_abi.h"
#include "platform/host_heap_synthetic.h"

#include "platform/host_log.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

namespace host {
namespace {

struct MemHeader {
    unsigned long size;
    unsigned long magic;
};

constexpr unsigned long kMemMagic = 0x53535833u;

unsigned long g_heap_bytes = 0;

} // namespace
} // namespace host

extern "C" {

void cMemMan_initialize(void* a0, void* a1, int a2, void* a3) {
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
    host::host_log("mem", "cMemMan_initialize");
}

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state) {
    (void)heap_tag;
    (void)flags;
    (void)heap_state;

    if (size == 0) {
        return nullptr;
    }

    void* block = std::malloc(size + 16);
    if (!block) {
        std::cerr << "[mem]     cMemMan_alloc failed for " << size << " bytes\n";
        return nullptr;
    }

    auto* header = static_cast<host::MemHeader*>(block);
    header->size = size;
    header->magic = host::kMemMagic;
    void* user = static_cast<char*>(block) + 16;
    host::g_heap_bytes += size;

    const std::string msg =
        "cMemMan_alloc " + std::to_string(size) + " bytes (heap " + std::to_string(host::g_heap_bytes) + ")";
    host::host_log("mem", msg.c_str());

    if (heap_tag != nullptr) {
        const auto tag = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(heap_tag));
        if ((tag & 0xF0000000u) == 0x20000000u) {
            HostHeapSyntheticBlock* block =
                static_cast<HostHeapSyntheticBlock*>(host_heap_synthetic_from_encoded(tag));
            if (block != nullptr) {
                block->word_at_54 = static_cast<int>(reinterpret_cast<uintptr_t>(user));
            }
        }
    }

    return user;
}

extern "C" void* func_003E6574(void* a0, void* heap_tag, int size) {
    (void)a0;
    return cMemMan_alloc(static_cast<unsigned long>(size), heap_tag, 0x20000000u, nullptr);
}

void cMemMan_free(void* ptr) {
    if (!ptr) {
        return;
    }

    auto* header = reinterpret_cast<host::MemHeader*>(static_cast<char*>(ptr) - 16);
    if (header->magic == host::kMemMagic) {
        host::g_heap_bytes -= header->size;
        header->magic = 0;
    }

    std::free(header);
}

} // extern "C"
