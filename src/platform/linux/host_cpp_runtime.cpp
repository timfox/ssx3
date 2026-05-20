#include <cstdlib>
#include <new>

/*
 * Minimal C++ runtime hooks for linking decompiled game TUs on the host.
 * Remove/replace when the standard library path is unified with decomp.
 */

void operator delete(void* ptr) noexcept { std::free(ptr); }

void operator delete(void* ptr, std::size_t) noexcept { std::free(ptr); }

void* operator new(std::size_t size) { return std::malloc(size); }

void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    return std::malloc(size);
}

extern "C" void __cxa_pure_virtual(void) {
    std::abort();
}

/* Decompiled crowdrender2d destructor calls this symbol name (not ::operator delete). */
void operator_delete(int* ptr) noexcept {
    ::operator delete(static_cast<void*>(ptr));
}
