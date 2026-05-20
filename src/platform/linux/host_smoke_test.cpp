#include "platform/host_disc.h"

#include <iostream>

int tHashName32_getHashValue(unsigned int* out, char* str);
int GetHashValue32(char* str);

namespace host {

namespace {

bool expect_hash32(const char* label, const char* input, unsigned int expected) {
    unsigned int hash = 0;
    const int result = tHashName32_getHashValue(&hash, const_cast<char*>(input));
    const int wrapped = GetHashValue32(const_cast<char*>(input));
    const bool ok = (hash == expected) && (result == static_cast<int>(expected)) &&
                    (wrapped == static_cast<int>(expected));
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "hash32 " << label << ": 0x" << std::hex << hash
              << std::dec << '\n';
    return ok;
}

} // namespace

bool run_smoke_tests() {
    std::cout << "host smoke tests (decompiled C on Linux)\n";

    bool ok = true;
    // Empty string: hash stays 0.
    ok = expect_hash32("empty", "", 0u) && ok;
    // "a" -> (0 << 4) + 97 = 97
    ok = expect_hash32("a", "a", 97u) && ok;

    const auto bootElf = disc().resolve_ps2_path("cdrom0:\\SLUS_207.72;1");
    if (bootElf && disc().exists("SLUS_207.72")) {
        std::cout << "[ok]      ps2 path resolve: " << *bootElf << '\n';
    } else {
        std::cout << "[fail]    ps2 path resolve for boot elf\n";
        ok = false;
    }

    const auto dataChildren = disc().list_children("data");
    if (dataChildren.empty()) {
        std::cout << "[warn]    disc/data has no entries (unexpected for a full rip)\n";
    } else {
        std::cout << "[ok]      disc/data entries: " << dataChildren.size() << '\n';
    }

    return ok;
}

} // namespace host
