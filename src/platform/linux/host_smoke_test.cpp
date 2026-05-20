#include "platform/host_big.h"
#include "platform/host_disc.h"

#include <iostream>

int tHashName32_getHashValue(unsigned int* out, char* str);
int GetHashValue32(char* str);
void BXsrand(unsigned int seed);
void BXrand();

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

bool smoke_big_audio() {
    auto* archive = big_open_cached("data/audio/audio.big");
    if (!archive) {
        std::cout << "[fail]    BIG open data/audio/audio.big\n";
        return false;
    }
    const BigEntry* entry = big_locate(*archive, "data\\audio\\BC_AmbLoop1.bnk");
    if (!entry || entry->size == 0) {
        std::cout << "[fail]    BIG_locate BC_AmbLoop1.bnk\n";
        return false;
    }
    std::cout << "[ok]      BIG audio.big: " << archive->entries.size() << " files, BC_AmbLoop1 @ 0x"
              << std::hex << entry->offset << " size=" << std::dec << entry->size << '\n';
    return true;
}

bool smoke_bxrandom() {
    BXsrand(1u);
    BXrand();
    BXrand();
    std::cout << "[ok]      bxrandom BXsrand/BXrand (decompiled + host BSS)\n";
    return true;
}

} // namespace

bool run_smoke_tests() {
    std::cout << "host smoke tests (decompiled C on Linux)\n";

    bool ok = true;
    ok = expect_hash32("empty", "", 0u) && ok;
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

    ok = smoke_big_audio() && ok;
    ok = smoke_bxrandom() && ok;

    return ok;
}

} // namespace host
