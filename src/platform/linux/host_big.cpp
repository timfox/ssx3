#include "platform/host_big.h"

#include "platform/host_disc.h"
#include "platform/host_log.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <unordered_map>

namespace host {
namespace {

constexpr char kBigfMagic[4] = {'B', 'I', 'G', 'F'};

std::uint32_t read_be32(const std::uint8_t* bytes) {
    return (static_cast<std::uint32_t>(bytes[0]) << 24) |
           (static_cast<std::uint32_t>(bytes[1]) << 16) |
           (static_cast<std::uint32_t>(bytes[2]) << 8) | static_cast<std::uint32_t>(bytes[3]);
}

std::string normalize_big_name(const char* name) {
    std::string out;
    if (!name) {
        return out;
    }
    for (const char* p = name; *p; ++p) {
        char ch = *p;
        if (ch == '\\') {
            ch = '/';
        }
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return out;
}

std::string normalize_entry_name(const std::string& name) {
    return normalize_big_name(name.c_str());
}

std::unordered_map<std::string, BigArchive> g_open_archives;

} // namespace

std::optional<BigArchive> big_open(const std::string& path) {
    std::filesystem::path resolved = path;
    if (!resolved.is_absolute()) {
        resolved = disc().resolve(resolved);
    }

    std::ifstream file(resolved, std::ios::binary);
    if (!file.is_open()) {
        return std::nullopt;
    }

    file.seekg(0, std::ios::end);
    const auto file_size = file.tellg();
    if (file_size < 16) {
        return std::nullopt;
    }
    file.seekg(0, std::ios::beg);

    BigArchive archive{};
    archive.path = resolved.string();
    archive.data.resize(static_cast<size_t>(file_size));
    file.read(reinterpret_cast<char*>(archive.data.data()), static_cast<std::streamsize>(file_size));
    if (file.gcount() != static_cast<std::streamsize>(file_size)) {
        return std::nullopt;
    }

    const auto* base = archive.data.data();
    if (std::memcmp(base, kBigfMagic, 4) != 0) {
        return std::nullopt;
    }

    const std::uint32_t nfiles = read_be32(base + 8);
    (void)read_be32(base + 12);

    size_t cursor = 16;
    archive.entries.reserve(nfiles);
    for (std::uint32_t i = 0; i < nfiles; ++i) {
        if (cursor + 8 > archive.data.size()) {
            break;
        }
        const std::uint8_t* entry = archive.data.data() + cursor;
        BigEntry e{};
        e.offset = read_be32(entry);
        e.size = read_be32(entry + 4);
        cursor += 8;
        const auto name_end = std::find(archive.data.begin() + static_cast<std::ptrdiff_t>(cursor),
                                        archive.data.end(),
                                        static_cast<std::uint8_t>(0));
        if (name_end == archive.data.end()) {
            break;
        }
        e.name.assign(reinterpret_cast<const char*>(archive.data.data() + cursor),
                      static_cast<size_t>(name_end - (archive.data.begin() + static_cast<std::ptrdiff_t>(cursor))));
        cursor = static_cast<size_t>(name_end - archive.data.begin()) + 1;
        archive.entries.push_back(std::move(e));
    }

    return archive;
}

BigArchive* big_open_cached(const std::string& path) {
    const std::string key = path;
    auto it = g_open_archives.find(key);
    if (it != g_open_archives.end()) {
        return &it->second;
    }
    auto opened = big_open(path);
    if (!opened) {
        return nullptr;
    }
    auto [placed, _] = g_open_archives.emplace(key, std::move(*opened));
    return &placed->second;
}

const BigEntry* big_locate(const BigArchive& archive, const char* name) {
    const std::string want = normalize_big_name(name);
    if (want.empty()) {
        return nullptr;
    }
    for (const BigEntry& entry : archive.entries) {
        if (normalize_entry_name(entry.name) == want) {
            return &entry;
        }
        const auto slash = entry.name.find_last_of("/\\");
        if (slash != std::string::npos) {
            const std::string base = entry.name.substr(slash + 1);
            if (normalize_entry_name(base) == want) {
                return &entry;
            }
        }
    }
    return nullptr;
}

} // namespace host

extern "C" {

unsigned long BIG_sizeofheader(const char* path) {
    if (!path) {
        return 0;
    }
    const auto archive = host::big_open(path);
    if (!archive || archive->data.size() < 16) {
        return 0;
    }
    return 16;
}

unsigned long BIG_typeofheader(const char* path) {
    if (!path) {
        return 0;
    }
    const auto archive = host::big_open(path);
    if (!archive || archive->data.size() < 4) {
        return 0;
    }
    return static_cast<unsigned long>(archive->data[0]) |
           (static_cast<unsigned long>(archive->data[1]) << 8) |
           (static_cast<unsigned long>(archive->data[2]) << 16) |
           (static_cast<unsigned long>(archive->data[3]) << 24);
}

void* BIG_locateentryz(void* archive, const char* name) {
    if (!archive || !name) {
        return nullptr;
    }
    auto* big = static_cast<host::BigArchive*>(archive);
    const host::BigEntry* entry = host::big_locate(*big, name);
    if (!entry) {
        return nullptr;
    }
    return const_cast<host::BigEntry*>(entry);
}

void BIG_debuginfo(void) { host::host_stub_log("BIG_debuginfo"); }

} // extern "C"
