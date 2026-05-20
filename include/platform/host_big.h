#ifndef PLATFORM_HOST_BIG_H
#define PLATFORM_HOST_BIG_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace host {

struct BigEntry {
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
    std::string name;
};

struct BigArchive {
    std::string path;
    std::vector<std::uint8_t> data;
    std::vector<BigEntry> entries;
};

/* Parse EA BIGF from disc path (host::disc() relative or absolute). */
std::optional<BigArchive> big_open(const std::string& path);

/* Case-insensitive path match; accepts '/' or '\\'. */
const BigEntry* big_locate(const BigArchive& archive, const char* name);

/* Cached open for repeated BIG_locateentryz calls (keyed by path string). */
BigArchive* big_open_cached(const std::string& path);

} // namespace host

#ifdef __cplusplus
extern "C" {
#endif

/* Retail BIGF helpers (host HAL). */
unsigned long BIG_sizeofheader(const char* path);
unsigned long BIG_typeofheader(const char* path);
void* BIG_locateentryz(void* archive, const char* name);
void BIG_debuginfo(void);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_BIG_H */
