#include "platform/host_fe_font.h"

#include "platform/host_disc.h"
#include "platform/host_log.h"
#include "platform/host_fe_layout.h"
#include "platform/host_menu_layout.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace fs = std::filesystem;

namespace host {
namespace {

constexpr char kMetricsMagic[4] = {'F', 'E', 'F', 'N'};
constexpr std::uint16_t kMetricsVersion = 1;

constexpr char kRetailAtlas[] = "fe_font_atlas.png";
constexpr char kRetailMetrics[] = "fe_font_metrics.bin";
constexpr char kCustomAtlas[] = "host_font_atlas.png";
constexpr char kCustomMetrics[] = "host_font_metrics.bin";

/* Retail FE maps "<start>" to the Start icon glyph (U+00A0 in fefont.sfn). */
constexpr char32_t kGlyphStartIcon = 0x00A0;
constexpr float kIconVirtualHeight = 34.0f;

struct FeFontGlyph {
    std::uint8_t width = 0;
    std::uint8_t height = 0;
    std::uint16_t u = 0;
    std::uint16_t v = 0;
    std::uint8_t advance = 0;
    std::int8_t x_offset = 0;
    std::int8_t y_offset = 0;
};

struct FeFontBank {
    bool loaded = false;
    int atlas_width = 0;
    int atlas_height = 0;
    int ascent = 0;
    int descent = 0;
    int texture_index = kTexFeFont;
    std::unordered_map<char32_t, FeFontGlyph> glyphs;
};

struct FeFontToken {
    const char* tag;
    char32_t code;
};

HostFontMode g_mode = HostFontMode::StbFallback;
HostFontChoice g_choice = HostFontChoice::Auto;
FeFontBank g_primary{};
FeFontBank g_icons{};

#pragma pack(push, 1)
struct PackedGlyph {
    std::uint16_t code;
    std::uint8_t width;
    std::uint8_t height;
    std::uint16_t u;
    std::uint16_t v;
    std::uint8_t advance;
    std::int8_t x_offset;
    std::int8_t y_offset;
};
#pragma pack(pop)
static_assert(sizeof(PackedGlyph) == 11, "FEFN glyph record must be 11 bytes");

const std::unordered_set<char32_t>& icon_codepoints() {
    static const std::unordered_set<char32_t> kIcons = {kGlyphStartIcon};
    return kIcons;
}

bool read_exact(std::ifstream& file, void* dst, std::size_t size) {
    file.read(reinterpret_cast<char*>(dst), static_cast<std::streamsize>(size));
    return file.gcount() == static_cast<std::streamsize>(size);
}

bool load_metrics_bin(const fs::path& path, FeFontBank& bank) {
    bank = FeFontBank{};
    bank.texture_index = kTexFeFont;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    char magic[4]{};
    if (!read_exact(file, magic, sizeof(magic)) || std::memcmp(magic, kMetricsMagic, 4) != 0) {
        return false;
    }

    std::uint16_t version = 0;
    std::uint32_t atlas_w = 0;
    std::uint32_t atlas_h = 0;
    std::uint8_t ascent = 0;
    std::uint8_t descent = 0;
    std::uint16_t glyph_count = 0;
    if (!read_exact(file, &version, sizeof(version)) || version != kMetricsVersion ||
        !read_exact(file, &atlas_w, sizeof(atlas_w)) || !read_exact(file, &atlas_h, sizeof(atlas_h)) ||
        !read_exact(file, &ascent, sizeof(ascent)) || !read_exact(file, &descent, sizeof(descent)) ||
        !read_exact(file, &glyph_count, sizeof(glyph_count))) {
        return false;
    }

    bank.atlas_width = static_cast<int>(atlas_w);
    bank.atlas_height = static_cast<int>(atlas_h);
    bank.ascent = ascent;
    bank.descent = descent;

    for (std::uint16_t i = 0; i < glyph_count; ++i) {
        PackedGlyph packed{};
        if (!read_exact(file, &packed, sizeof(packed))) {
            std::cerr << "[font]    truncated glyph " << i << " in " << path.filename() << '\n';
            bank = FeFontBank{};
            return false;
        }
        FeFontGlyph glyph{};
        glyph.width = packed.width;
        glyph.height = packed.height;
        glyph.u = packed.u;
        glyph.v = packed.v;
        glyph.advance = packed.advance;
        glyph.x_offset = packed.x_offset;
        glyph.y_offset = packed.y_offset;
        bank.glyphs[static_cast<char32_t>(packed.code)] = glyph;
    }

    bank.loaded = bank.atlas_width > 0 && bank.atlas_height > 0 && !bank.glyphs.empty();
    return bank.loaded;
}

bool fe_font_next_codepoint(const char*& cursor, char32_t& code) {
    if (!cursor || *cursor == '\0') {
        return false;
    }

    static constexpr FeFontToken kTokens[] = {
        {"<start>", kGlyphStartIcon},
    };
    for (const FeFontToken& token : kTokens) {
        const std::size_t len = std::strlen(token.tag);
        if (std::strncmp(cursor, token.tag, len) == 0) {
            code = token.code;
            cursor += len;
            return true;
        }
    }

    code = static_cast<unsigned char>(*cursor);
    ++cursor;
    return true;
}

bool is_icon_code(char32_t code) {
    return icon_codepoints().count(code) != 0;
}

const FeFontBank& bank_for_glyph(char32_t code) {
    if (is_icon_code(code) && g_icons.loaded && g_mode == HostFontMode::CustomWithRetailIcons) {
        return g_icons;
    }
    return g_primary;
}

float fe_font_advance_virtual(const FeFontGlyph& glyph, char32_t code, float scale) {
    if (is_icon_code(code)) {
        return kIconVirtualHeight * 0.92f * scale;
    }
    float advance = static_cast<float>(glyph.advance) * scale;
    if (static_cast<float>(glyph.width) * scale > advance) {
        advance = static_cast<float>(glyph.width) * scale;
    }
    return advance;
}

bool lookup_glyph(char32_t code, const FeFontGlyph*& glyph, int& texture_index) {
    const FeFontBank& bank = bank_for_glyph(code);
    if (!bank.loaded) {
        return false;
    }
    const auto it = bank.glyphs.find(code);
    if (it == bank.glyphs.end()) {
        return false;
    }
    glyph = &it->second;
    texture_index = bank.texture_index;
    return true;
}

void emit_glyph_sprite(std::vector<MenuSprite>& sprites,
                       const MenuViewport& viewport,
                       const FeFontGlyph& glyph,
                       int texture_index,
                       char32_t code,
                       float pen_x,
                       float baseline_y,
                       float scale,
                       float scale_x,
                       float scale_y,
                       float atlas_w,
                       float atlas_h,
                       float r,
                       float g,
                       float b,
                       float a) {
    if (glyph.width == 0 || glyph.height == 0) {
        return;
    }

    float draw_w = std::max(static_cast<float>(glyph.width) * scale_x, 1.0f);
    float draw_h = std::max(static_cast<float>(glyph.height) * scale_y, 1.0f);
    float x_off = static_cast<float>(glyph.x_offset) * scale;
    float y_off = static_cast<float>(glyph.y_offset) * scale;

    if (is_icon_code(code)) {
        draw_h = std::max((kIconVirtualHeight / static_cast<float>(kTitleVirtualHeight)) * viewport.height,
                          1.0f);
        draw_w = draw_h * (static_cast<float>(glyph.width) / static_cast<float>(glyph.height));
        x_off = 0.0f;
        const float cap_h =
            std::max((20.0f / static_cast<float>(kTitleVirtualHeight)) * viewport.height, 1.0f);
        y_off = (cap_h - draw_h) * 0.5f;
    }

    MenuSprite sprite{};
    sprite.texture_index = texture_index;
    sprite.x = viewport.x + ((pen_x + x_off) / static_cast<float>(kTitleVirtualWidth)) * viewport.width;
    sprite.y =
        viewport.y + ((baseline_y + y_off) / static_cast<float>(kTitleVirtualHeight)) * viewport.height;
    sprite.w = draw_w;
    sprite.h = draw_h;
    sprite.u0 = static_cast<float>(glyph.u) / atlas_w;
    sprite.v0 = static_cast<float>(glyph.v) / atlas_h;
    sprite.u1 = static_cast<float>(glyph.u + glyph.width) / atlas_w;
    sprite.v1 = static_cast<float>(glyph.v + glyph.height) / atlas_h;
    sprite.r = r;
    sprite.g = g;
    sprite.b = b;
    sprite.a = a;
    sprites.push_back(sprite);
}

} // namespace

void host_fe_font_set_choice(HostFontChoice choice) {
    g_choice = choice;
}

bool host_fe_font_custom_ready(const std::string& asset_dir) {
    const fs::path root(asset_dir);
    return fs::is_regular_file(root / kCustomAtlas) && fs::is_regular_file(root / kCustomMetrics);
}

bool host_fe_font_ready(const std::string& asset_dir) {
    const fs::path root(asset_dir);
    return fs::is_regular_file(root / kRetailAtlas) && fs::is_regular_file(root / kRetailMetrics);
}

bool host_fe_font_ensure(const std::string& asset_dir) {
    if (host_fe_font_ready(asset_dir)) {
        return true;
    }

    const fs::path font_path = disc().resolve("data/fonts/fefont.sfn");
    if (!fs::is_regular_file(font_path)) {
        host_log("font", "missing disc data/fonts/fefont.sfn");
        return false;
    }

    const fs::path disc_root = disc().root();
    const std::string cmd =
        "python3 scripts/extract_menu_font.py --disc \"" + disc_root.string() + "\"";
    host_log("font", "extracting retail FE font (fefont.sfn) …");
    std::cout << "[font]    " << cmd << '\n';
    const int rc = std::system(cmd.c_str());
    if (rc != 0) {
        std::cerr << "[font]    extract failed (exit " << rc << ")\n";
        return false;
    }

    if (!host_fe_font_ready(asset_dir)) {
        host_log("font", "extract finished but fe_font_atlas.png still missing");
        return false;
    }
    return true;
}

bool host_fe_font_load(const std::string& asset_dir) {
    g_primary = FeFontBank{};
    g_icons = FeFontBank{};
    g_mode = HostFontMode::StbFallback;

    const fs::path root(asset_dir);
    const bool have_retail = host_fe_font_ready(asset_dir);
    const bool have_custom = host_fe_font_custom_ready(asset_dir);

    const bool want_custom = g_choice == HostFontChoice::Custom;
    const bool want_retail = g_choice == HostFontChoice::Retail;
    /* Auto: retail FE font matches SLUS menus; custom is opt-in via --font custom. */
    const bool try_custom = want_custom || (g_choice == HostFontChoice::Auto && have_custom && !have_retail);
    const bool try_retail = want_retail || (g_choice == HostFontChoice::Auto && have_retail) || !g_primary.loaded;

    if (have_retail && (want_custom || (try_custom && have_custom))) {
        load_metrics_bin(root / kRetailMetrics, g_icons);
        g_icons.texture_index = kTexFeIcons;
    }

    if (try_custom && have_custom) {
        if (load_metrics_bin(root / kCustomMetrics, g_primary)) {
            g_primary.texture_index = kTexFeFont;
            g_mode = (g_icons.loaded && have_retail) ? HostFontMode::CustomWithRetailIcons
                                                     : HostFontMode::Custom;
        } else {
            std::cerr << "[font]    host_font_metrics.bin invalid — remove it or rerun bake_host_font.py\n";
        }
    }

    if (!g_primary.loaded && try_retail && have_retail) {
        if (load_metrics_bin(root / kRetailMetrics, g_primary)) {
            g_primary.texture_index = kTexFeFont;
            g_mode = HostFontMode::Retail;
            g_icons = FeFontBank{};
        } else {
            std::cerr << "[font]    fe_font_metrics.bin invalid — rerun extract_menu_font.py\n";
        }
    }

    if (g_primary.loaded) {
        std::cout << "[font]    mode=" << host_fe_font_mode_name() << " glyphs=" << g_primary.glyphs.size()
                  << " atlas=" << g_primary.atlas_width << "x" << g_primary.atlas_height;
        if (g_mode == HostFontMode::CustomWithRetailIcons) {
            std::cout << " (retail icons=" << g_icons.glyphs.size() << ")";
        }
        std::cout << '\n';
        host_log("font", host_fe_font_mode_name());
    }

    return g_primary.loaded;
}

bool host_fe_font_is_loaded() {
    return g_primary.loaded;
}

HostFontMode host_fe_font_mode() {
    return g_mode;
}

const char* host_fe_font_mode_name() {
    switch (g_mode) {
    case HostFontMode::Retail:
        return "retail";
    case HostFontMode::Custom:
        return "custom";
    case HostFontMode::CustomWithRetailIcons:
        return "custom+retail-icons";
    case HostFontMode::StbFallback:
    default:
        return "stb-fallback";
    }
}

bool host_fe_font_gpu_paths(const std::string& asset_dir,
                            std::string& primary_png,
                            std::string& icons_png,
                            bool& load_icons) {
    primary_png.clear();
    icons_png.clear();
    load_icons = false;

    if (!g_primary.loaded) {
        return false;
    }

    const fs::path root(asset_dir);
    if (g_mode == HostFontMode::Custom || g_mode == HostFontMode::CustomWithRetailIcons) {
        primary_png = (root / kCustomAtlas).string();
    } else {
        primary_png = (root / kRetailAtlas).string();
    }

    if (g_mode == HostFontMode::CustomWithRetailIcons) {
        icons_png = (root / kRetailAtlas).string();
        load_icons = true;
    }

    return fs::is_regular_file(primary_png);
}

float fe_font_line_height_virtual(float scale) {
    const int line = std::max(g_primary.ascent + g_primary.descent, 20);
    return static_cast<float>(line) * scale;
}

void append_fe_text(std::vector<MenuSprite>& sprites,
                    const MenuViewport& viewport,
                    const char* text,
                    float vx,
                    float vy,
                    float scale,
                    float r,
                    float g,
                    float b,
                    float a) {
    if (!text || !g_primary.loaded || g_primary.atlas_width <= 0 || g_primary.atlas_height <= 0) {
        return;
    }

    const float scale_x = (scale / static_cast<float>(kTitleVirtualWidth)) * viewport.width;
    const float scale_y = (scale / static_cast<float>(kTitleVirtualHeight)) * viewport.height;

    float pen_x = vx;
    const int line_ascent = g_primary.ascent > 0 ? g_primary.ascent : 20;
    const float baseline_y = vy + static_cast<float>(line_ascent) * scale;

    const char* cursor = text;
    char32_t code = 0;
    while (fe_font_next_codepoint(cursor, code)) {
        if (code == '\n') {
            pen_x = vx;
            continue;
        }

        const FeFontGlyph* glyph = nullptr;
        int texture_index = kTexFeFont;
        if (!lookup_glyph(code, glyph, texture_index)) {
            const FeFontGlyph* space_glyph = nullptr;
            int space_tex = kTexFeFont;
            if (lookup_glyph(U' ', space_glyph, space_tex)) {
                pen_x += static_cast<float>(space_glyph->advance) * scale;
            } else {
                pen_x += 6.0f * scale;
            }
            continue;
        }

        const FeFontBank& bank = bank_for_glyph(code);
        const float bank_atlas_w = static_cast<float>(bank.atlas_width);
        const float bank_atlas_h = static_cast<float>(bank.atlas_height);

        if (glyph->width == 0 || glyph->height == 0) {
            pen_x += fe_font_advance_virtual(*glyph, code, scale);
            continue;
        }

        emit_glyph_sprite(sprites,
                          viewport,
                          *glyph,
                          texture_index,
                          code,
                          pen_x,
                          baseline_y,
                          scale,
                          scale_x,
                          scale_y,
                          bank_atlas_w,
                          bank_atlas_h,
                          r,
                          g,
                          b,
                          a);

        pen_x += fe_font_advance_virtual(*glyph, code, scale);
    }
}

float measure_fe_text_width_virtual(const char* text, float scale) {
    if (!text || !g_primary.loaded) {
        return 0.0f;
    }

    float width = 0.0f;
    const char* cursor = text;
    char32_t code = 0;
    while (fe_font_next_codepoint(cursor, code)) {
        if (code == '\n') {
            break;
        }

        const FeFontGlyph* glyph = nullptr;
        int texture_index = kTexFeFont;
        if (!lookup_glyph(code, glyph, texture_index)) {
            const FeFontGlyph* space_glyph = nullptr;
            int space_tex = kTexFeFont;
            if (lookup_glyph(U' ', space_glyph, space_tex)) {
                width += static_cast<float>(space_glyph->advance) * scale;
            } else {
                width += 6.0f * scale;
            }
        } else {
            width += fe_font_advance_virtual(*glyph, code, scale);
        }
    }
    return width;
}

} // namespace host
