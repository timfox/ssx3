#include "platform/host_fe_locale.h"

#include "platform/host_log.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace host {
namespace {

std::unordered_map<std::string, std::string> g_curated;

bool try_load_json(const std::filesystem::path& path) {
    std::ifstream in(path);
    if (!in) {
        return false;
    }
    std::ostringstream buf;
    buf << in.rdbuf();
    const std::string json = buf.str();

    g_curated.clear();
    size_t pos = 0;
    while (pos < json.size()) {
        const size_t key_start = json.find('"', pos);
        if (key_start == std::string::npos) {
            break;
        }
        size_t key_end = key_start + 1;
        while (key_end < json.size()) {
            if (json[key_end] == '"' && json[key_end - 1] != '\\') {
                break;
            }
            ++key_end;
        }
        if (key_end >= json.size()) {
            break;
        }
        const std::string key = json.substr(key_start + 1, key_end - key_start - 1);

        const size_t colon = json.find(':', key_end);
        if (colon == std::string::npos) {
            break;
        }
        const size_t val_start = json.find('"', colon);
        if (val_start == std::string::npos) {
            break;
        }
        size_t val_end = val_start + 1;
        std::string value;
        while (val_end < json.size()) {
            const char ch = json[val_end];
            if (ch == '"' && json[val_end - 1] != '\\') {
                break;
            }
            if (ch == '\\' && val_end + 1 < json.size()) {
                const char next = json[val_end + 1];
                if (next == 'n') {
                    value.push_back('\n');
                    val_end += 2;
                    continue;
                }
                if (next == '\\' || next == '"') {
                    value.push_back(next);
                    val_end += 2;
                    continue;
                }
            }
            value.push_back(ch);
            ++val_end;
        }
        g_curated[key] = value;
        pos = val_end + 1;
    }
    return !g_curated.empty();
}

} // namespace

bool fe_locale_init() {
    const std::filesystem::path candidates[] = {
        "assets/host/locale/fe_strings.json",
        "../assets/host/locale/fe_strings.json",
    };
    for (const auto& path : candidates) {
        if (try_load_json(path)) {
            host_log("locale",
                     ("curated " + std::to_string(g_curated.size()) + " keys from " + path.string()).c_str());
            return true;
        }
    }
    host_log("locale", "fe_strings.json missing — run: python3 scripts/extract_locale.py --disc disc");
    return false;
}

const char* fe_locale_get(const char* key) {
    const auto it = g_curated.find(key ? key : "");
    if (it == g_curated.end()) {
        return nullptr;
    }
    return it->second.c_str();
}

} // namespace host
