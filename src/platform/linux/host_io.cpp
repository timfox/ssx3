#include "platform/host_io.h"

#include "platform/host_disc.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>

namespace host {
namespace {

std::string to_lower(std::string value) {
    for (char& ch : value) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return value;
}

} // namespace

std::optional<std::string> ps2_path_to_disc_relative(const std::string& ps2_path) {
    const auto resolved = disc().resolve_ps2_path(ps2_path);
    if (!resolved) {
        return std::nullopt;
    }
    const auto& root = disc().root();
    std::error_code error;
    const auto rel = std::filesystem::relative(*resolved, root, error);
    if (error) {
        return resolved->generic_string();
    }
    return rel.generic_string();
}

std::optional<std::vector<std::uint8_t>> read_disc_file(const std::string& disc_relative) {
    const auto path = disc().resolve(disc_relative);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return std::nullopt;
    }
    file.seekg(0, std::ios::end);
    const auto size = file.tellg();
    if (size <= 0) {
        return std::nullopt;
    }
    std::vector<std::uint8_t> bytes(static_cast<size_t>(size));
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(size));
    if (file.gcount() != static_cast<std::streamsize>(size)) {
        return std::nullopt;
    }
    return bytes;
}

std::optional<std::vector<std::uint8_t>> read_big_entry(const BigArchive& archive,
                                                        const char* entry_name) {
    const BigEntry* entry = big_locate(archive, entry_name);
    if (!entry || entry->size == 0) {
        return std::nullopt;
    }
    if (entry->offset + entry->size > archive.data.size()) {
        return std::nullopt;
    }
    std::vector<std::uint8_t> bytes(entry->size);
    std::memcpy(bytes.data(), archive.data.data() + entry->offset, entry->size);
    return bytes;
}

std::optional<std::vector<std::uint8_t>> read_asset_bytes(const std::string& asset_path) {
    std::string path = asset_path;
    for (char& ch : path) {
        if (ch == '\\') {
            ch = '/';
        }
    }

    if (auto loose = read_disc_file(path)) {
        return loose;
    }

    const std::string lower = to_lower(path);
    const auto pos = lower.find(".big/");
    if (pos == std::string::npos) {
        return std::nullopt;
    }

    const std::string archive_path = path.substr(0, pos + 4);
    const std::string entry_path = path.substr(pos + 5);
    auto* archive = big_open_cached(archive_path);
    if (!archive) {
        return std::nullopt;
    }
    return read_big_entry(*archive, entry_path.c_str());
}

} // namespace host
