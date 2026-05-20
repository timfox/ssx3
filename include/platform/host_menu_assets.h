#ifndef PLATFORM_HOST_MENU_ASSETS_H
#define PLATFORM_HOST_MENU_ASSETS_H

#include <string>

namespace host {

/* True when title_logo.png or ui_ssxt.png exists under asset_dir. */
bool host_menu_assets_ready(const std::string& asset_dir);

/*
 * If assets are missing but disc/data/ui/fe_1.ssh exists, run
 * scripts/extract_menu_assets.py (requires EA-Graphics-Manager in /tmp).
 */
bool host_menu_ensure_assets(const std::string& asset_dir);

} // namespace host

#endif /* PLATFORM_HOST_MENU_ASSETS_H */
