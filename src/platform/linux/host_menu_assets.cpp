#include "platform/host_menu_assets.h"

#include "platform/host_disc.h"
#include "platform/host_log.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace host {

bool host_menu_assets_ready(const std::string& asset_dir) {
    const fs::path root(asset_dir);
    return fs::is_regular_file(root / "title_logo.png") ||
           fs::is_regular_file(root / "ui_ssxt.png");
}

bool host_menu_ensure_assets(const std::string& asset_dir) {
    if (host_menu_assets_ready(asset_dir)) {
        return true;
    }

    const fs::path ssh = disc().resolve("data/ui/fe_1.ssh");
    if (!fs::is_regular_file(ssh)) {
        host_log("menu", "missing disc data/ui/fe_1.ssh — cannot extract title art");
        return false;
    }

    const fs::path disc_root = disc().root();
    const std::string cmd =
        "python3 scripts/extract_menu_assets.py --disc \"" + disc_root.string() + "\"";
    host_log("menu", "extracting title assets from fe_1.ssh …");
    std::cout << "[menu]    " << cmd << '\n';
    const int rc = std::system(cmd.c_str());
    if (rc != 0) {
        std::cerr << "[menu]    extract failed (exit " << rc
                  << "); clone EA-Graphics-Manager to /tmp/EA-Graphics-Manager\n";
        return false;
    }

    if (!host_menu_assets_ready(asset_dir)) {
        host_log("menu", "extract finished but title_logo.png still missing");
        return false;
    }
    host_log("menu", ("fe_1.ssh assets ready under " + asset_dir).c_str());
    return true;
}

} // namespace host
