#include "platform/host_fe_layout_runtime.h"

#include "platform/host_log.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

namespace host {
namespace {

FeMainMenuLayoutValues g_main_menu{};
FeMountainLayoutValues g_mountain{};
bool g_loaded = false;

bool parse_float(const std::string& json, const char* key, float& out) {
    const std::string needle = std::string("\"") + key + "\"";
    const size_t pos = json.find(needle);
    if (pos == std::string::npos) {
        return false;
    }
    size_t cursor = pos + needle.size();
    while (cursor < json.size() && json[cursor] != ':') {
        ++cursor;
    }
    if (cursor >= json.size() || json[cursor] != ':') {
        return false;
    }
    ++cursor;
    while (cursor < json.size() && std::isspace(static_cast<unsigned char>(json[cursor]))) {
        ++cursor;
    }
    char* end = nullptr;
    const float value = std::strtof(json.c_str() + cursor, &end);
    if (end == json.c_str() + cursor) {
        return false;
    }
    out = value;
    return true;
}

std::string extract_section(const std::string& json, const char* section) {
    const std::string open = std::string("\"") + section + "\"";
    const size_t sec = json.find(open);
    if (sec == std::string::npos) {
        return {};
    }
    const size_t brace = json.find('{', sec);
    if (brace == std::string::npos) {
        return {};
    }
    size_t depth = 0;
    for (size_t end = brace; end < json.size(); ++end) {
        if (json[end] == '{') {
            ++depth;
        } else if (json[end] == '}') {
            --depth;
            if (depth == 0) {
                return json.substr(brace, end - brace + 1);
            }
        }
    }
    return {};
}

bool load_main_menu_section(const std::string& slice) {
    bool any = false;
    auto key = [&](const char* name, float& field) {
        if (parse_float(slice, name, field)) {
            any = true;
        }
    };
    key("header_divider_y", g_main_menu.header_divider_y);
    key("column_divider_x", g_main_menu.column_divider_x);
    key("orange_three_x", g_main_menu.orange_three_x);
    key("orange_three_y", g_main_menu.orange_three_y);
    key("header_title_x", g_main_menu.header_title_x);
    key("header_title_y", g_main_menu.header_title_y);
    key("header_title_scale", g_main_menu.header_title_scale);
    key("corner_logo_w", g_main_menu.corner_logo_w);
    key("corner_logo_x", g_main_menu.corner_logo_x);
    key("corner_logo_y", g_main_menu.corner_logo_y);
    key("desc_x", g_main_menu.desc_x);
    key("desc_y", g_main_menu.desc_y);
    key("desc_scale", g_main_menu.desc_scale);
    key("menu_x", g_main_menu.menu_x);
    key("menu_y", g_main_menu.menu_y);
    key("menu_line_h", g_main_menu.menu_line_h);
    key("menu_scale", g_main_menu.menu_scale);
    key("menu_scale_sel", g_main_menu.menu_scale_sel);
    key("sel_bar_x", g_main_menu.sel_bar_x);
    key("sel_bar_w", g_main_menu.sel_bar_w);
    key("sel_bar_h", g_main_menu.sel_bar_h);
    key("mountain_x", g_main_menu.mountain_x);
    key("mountain_y", g_main_menu.mountain_y);
    key("mountain_w", g_main_menu.mountain_w);
    key("mountain_h", g_main_menu.mountain_h);
    key("hint_x", g_main_menu.hint_x);
    key("hint_y", g_main_menu.hint_y);
    key("hint_scale", g_main_menu.hint_scale);
    return any;
}

bool load_mountain_section(const std::string& slice) {
    bool any = false;
    auto key = [&](const char* name, float& field) {
        if (parse_float(slice, name, field)) {
            any = true;
        }
    };
    key("header_scale", g_mountain.header_scale);
    key("header_x", g_mountain.header_x);
    key("header_y", g_mountain.header_y);
    key("peak_scale", g_mountain.peak_scale);
    key("peak_scale_sel", g_mountain.peak_scale_sel);
    key("peak_y", g_mountain.peak_y);
    key("peak_card_w", g_mountain.peak_card_w);
    key("peak_gap", g_mountain.peak_gap);
    key("map_panorama_x", g_mountain.map_panorama_x);
    key("map_panorama_y", g_mountain.map_panorama_y);
    key("map_panorama_w", g_mountain.map_panorama_w);
    key("map_panorama_h", g_mountain.map_panorama_h);
    return any;
}

} // namespace

bool fe_layout_load() {
    if (g_loaded) {
        return true;
    }
    const char* paths[] = {
        "assets/host/ui/fe_layout.json",
        "../assets/host/ui/fe_layout.json",
    };
    std::string json;
    for (const char* path : paths) {
        std::ifstream in(path);
        if (!in) {
            continue;
        }
        std::ostringstream buf;
        buf << in.rdbuf();
        json = buf.str();
        break;
    }
    g_loaded = true;
    if (json.empty()) {
        return false;
    }
    const bool main_ok = load_main_menu_section(extract_section(json, "main_menu"));
    const bool mtn_ok = load_mountain_section(extract_section(json, "mountain_room"));
    if (main_ok || mtn_ok) {
        host_log("layout", "loaded assets/host/ui/fe_layout.json");
    }
    return main_ok || mtn_ok;
}

const FeMainMenuLayoutValues& fe_main_menu_layout() {
    if (!g_loaded) {
        fe_layout_load();
    }
    return g_main_menu;
}

const FeMountainLayoutValues& fe_mountain_layout() {
    if (!g_loaded) {
        fe_layout_load();
    }
    return g_mountain;
}

} // namespace host
