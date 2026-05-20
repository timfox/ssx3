#include "platform/host_abi.h"

#include "platform/host_log.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

namespace host {
namespace {

struct MemHeader {
    unsigned long size;
    unsigned long magic;
};

constexpr unsigned long kMemMagic = 0x53535833u;

std::unordered_map<void*, MemHeader> g_allocations;
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
    host::g_allocations[user] = *header;
    host::g_heap_bytes += size;

    const std::string msg =
        "cMemMan_alloc " + std::to_string(size) + " bytes (heap " + std::to_string(host::g_heap_bytes) + ")";
    host::host_log("mem", msg.c_str());
    return user;
}

void cMemMan_free(void* ptr) {
    if (!ptr) {
        return;
    }

    auto it = host::g_allocations.find(ptr);
    if (it != host::g_allocations.end()) {
        host::g_heap_bytes -= it->second.size;
        host::g_allocations.erase(it);
    }

    void* block = static_cast<char*>(ptr) - 16;
    std::free(block);
}

} // extern "C"
