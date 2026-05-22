// Retail cFELocale @ 0x1959A8 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_cfelocale.h"
#include "platform/host_fe_locale.h"
#include "platform/host_io.h"
#include "platform/host_log.h"

#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" int tHashName32_getHashValue(unsigned int* out, char* str);

namespace host {
namespace {

std::vector<std::string> g_loc_storage;
std::unordered_map<std::string, std::size_t> g_loc_by_text;
std::unordered_map<unsigned int, std::size_t> g_loc_by_hash;

void store_loc_string(std::string text) {
    if (text.empty()) {
        return;
    }
    const auto existing = g_loc_by_text.find(text);
    if (existing != g_loc_by_text.end()) {
        return;
    }
    g_loc_storage.push_back(std::move(text));
    const std::size_t index = g_loc_storage.size() - 1;
    const std::string& stored = g_loc_storage[index];
    g_loc_by_text.emplace(stored, index);

    unsigned int hash = 0;
    tHashName32_getHashValue(&hash, const_cast<char*>(stored.c_str()));
    g_loc_by_hash.emplace(hash, index);
}

int merge_loc_bytes(const std::vector<std::uint8_t>& data) {
    int added = 0;
    std::size_t i = 0;
    while (i + 3 < data.size()) {
        if (data[i + 1] != 0 || !std::isprint(static_cast<unsigned char>(data[i]))) {
            ++i;
            continue;
        }
        const std::size_t start = i;
        std::string text;
        while (i + 1 < data.size()) {
            const unsigned char lo = data[i];
            const unsigned char hi = data[i + 1];
            if (hi != 0 || !std::isprint(lo)) {
                break;
            }
            text.push_back(static_cast<char>(lo));
            i += 2;
        }
        if (text.size() >= 2) {
            const std::size_t before = g_loc_storage.size();
            store_loc_string(std::move(text));
            if (g_loc_storage.size() > before) {
                ++added;
            }
        }
        if (i == start) {
            ++i;
        }
    }
    return added;
}

std::string normalize_loc_path(const char* path) {
    std::string out = path ? path : "";
    for (char& ch : out) {
        if (ch == '\\') {
            ch = '/';
        }
    }
    if (out.find("data/") == 0 || out.find("DATA/") == 0) {
        return out;
    }
    if (out.find("locale/") != std::string::npos) {
        return "data/" + out;
    }
    return out;
}

} // namespace

int fe_locale_add_loc_file(const char* path) {
    if (!path || !path[0]) {
        return 0;
    }
    const std::string disc_path = normalize_loc_path(path);
    const auto bytes = read_disc_file(disc_path);
    if (!bytes) {
        host_log("locale", ("cFELocale_addFile: could not read " + disc_path).c_str());
        return 0;
    }
    const int added = merge_loc_bytes(*bytes);
    char log_buf[128];
    std::snprintf(log_buf,
                  sizeof(log_buf),
                  "cFELocale_addFile %s: +%d strings (pool %zu)",
                  disc_path.c_str(),
                  added,
                  g_loc_storage.size());
    host_log("locale", log_buf);
    return added;
}

const char* fe_locale_by_text(const char* text) {
    if (!text) {
        return nullptr;
    }
    const auto it = g_loc_by_text.find(text);
    if (it == g_loc_by_text.end()) {
        return nullptr;
    }
    return g_loc_storage[it->second].c_str();
}

const char* fe_locale_by_hash(unsigned int hash) {
    const auto it = g_loc_by_hash.find(hash);
    if (it == g_loc_by_hash.end()) {
        return nullptr;
    }
    return g_loc_storage[it->second].c_str();
}

} // namespace host

extern "C" {
#endif

int cFELocale_addFile(void* locale, const char* path) {
    (void)locale;
    return host::fe_locale_add_loc_file(path) > 0 ? 1 : 0;
}

const char* cFELocale_getString(void* locale, unsigned int hash_id) {
    (void)locale;
    return host::fe_locale_by_hash(hash_id);
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
