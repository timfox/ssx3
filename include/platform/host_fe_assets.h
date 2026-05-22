#ifndef PLATFORM_HOST_FE_ASSETS_H
#define PLATFORM_HOST_FE_ASSETS_H

#include <string>

namespace host {

constexpr const char kHostAssetsRoot[] = "assets/host";

std::string fe_menu_asset_dir();
std::string fe_map_asset_dir();
std::string fe_characters_asset_dir();
std::string fe_locale_asset_dir();

/** Ensure menu PNGs exist (may run extract_menu_assets.py). */
bool fe_ensure_menu_assets();

/** Ensure fe_strings.json exists (may run extract_locale.py). */
bool fe_ensure_locale_assets();

/** Run extract_host_assets.py when disc is present and assets are incomplete. */
bool fe_ensure_host_assets();

} // namespace host

#endif /* PLATFORM_HOST_FE_ASSETS_H */
