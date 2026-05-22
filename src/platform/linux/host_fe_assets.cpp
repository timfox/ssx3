#include "platform/host_fe_assets.h"

#include "platform/host_disc.h"
#include "platform/host_log.h"
#include "platform/host_menu_assets.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace host {

std::string fe_menu_asset_dir() { return std::string(kHostAssetsRoot) + "/menu"; }

std::string fe_map_asset_dir() { return std::string(kHostAssetsRoot) + "/map"; }

std::string fe_characters_asset_dir() { return std::string(kHostAssetsRoot) + "/characters"; }

std::string fe_locale_asset_dir() { return std::string(kHostAssetsRoot) + "/locale"; }

bool fe_ensure_menu_assets() { return host_menu_ensure_assets(fe_menu_asset_dir()); }

bool fe_ensure_locale_assets() {
    const fs::path strings = fs::path(fe_locale_asset_dir()) / "fe_strings.json";
    if (fs::is_regular_file(strings)) {
        return true;
    }
    const fs::path disc_root = disc().root();
    if (!fs::is_regular_file(disc_root / "SYSTEM.CNF")) {
        host_log("assets", "fe_strings.json missing and no disc for extract_locale.py");
        return false;
    }
    const std::string cmd =
        "python3 scripts/extract_locale.py --disc \"" + disc_root.string() + "\"";
    host_log("assets", "extracting locale …");
    std::cout << "[assets]  " << cmd << '\n';
    if (std::system(cmd.c_str()) != 0) {
        return false;
    }
    return fs::is_regular_file(strings);
}

bool fe_ensure_ui_textures() {
    const fs::path ui_bg = fs::path(kHostAssetsRoot) / "ui" / "fe_1_EA_b.png";
    const fs::path menu_panel = fs::path(fe_menu_asset_dir()) / "menu_panel.png";
    if (fs::is_regular_file(ui_bg) && fs::is_regular_file(menu_panel)) {
        return true;
    }
    const fs::path disc_root = disc().root();
    if (!fs::is_regular_file(disc_root / "SYSTEM.CNF")) {
        return fs::is_regular_file(menu_panel);
    }
    const std::string cmd =
        "python3 scripts/extract_ui_archives.py --disc \"" + disc_root.string() + "\" --only fe_1";
    host_log("assets", "extracting FE UI textures (fe_1.ssh) …");
    std::cout << "[assets]  " << cmd << '\n';
    return std::system(cmd.c_str()) == 0;
}

bool fe_ensure_host_assets() {
    bool ok = fe_ensure_menu_assets();
    ok = fe_ensure_ui_textures() && ok;
    ok = fe_ensure_locale_assets() && ok;
    const fs::path map_mtn = fs::path(fe_map_asset_dir()) / "map_mtn_mout.png";
    if (!fs::is_regular_file(map_mtn)) {
        const fs::path disc_root = disc().root();
        if (fs::is_regular_file(disc_root / "SYSTEM.CNF")) {
            const std::string cmd =
                "python3 scripts/extract_ui_big.py --disc \"" + disc_root.string() +
                "\" --flat-only --only map characters";
            host_log("assets", "extracting map/character UI …");
            std::cout << "[assets]  " << cmd << '\n';
            (void)std::system(cmd.c_str());
        }
    }
    return ok;
}

} // namespace host
