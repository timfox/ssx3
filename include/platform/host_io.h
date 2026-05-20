#ifndef PLATFORM_HOST_IO_H
#define PLATFORM_HOST_IO_H

#include "platform/host_big.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace host {

/* Resolve cdrom0:\\data\\foo;1 style paths to disc-relative paths. */
std::optional<std::string> ps2_path_to_disc_relative(const std::string& ps2_path);

/* Read a loose file under disc/ (returns empty if missing). */
std::optional<std::vector<std::uint8_t>> read_disc_file(const std::string& disc_relative);

/*
 * Read bytes from a path that may be a loose disc file or a BIG archive path.
 * BIG paths use "archive.big/path/inside/file.ext" (archive ends with .big).
 */
std::optional<std::vector<std::uint8_t>> read_asset_bytes(const std::string& asset_path);

std::optional<std::vector<std::uint8_t>> read_big_entry(const BigArchive& archive,
                                                        const char* entry_name);

} // namespace host

#endif /* PLATFORM_HOST_IO_H */
