#ifndef PLATFORM_HOST_MAIN_MENU_H
#define PLATFORM_HOST_MAIN_MENU_H

#include "platform/host_renderer.h"

#include <string>

namespace host {

bool run_main_menu_loop(Renderer* renderer, const std::string& asset_dir);

} // namespace host

#endif /* PLATFORM_HOST_MAIN_MENU_H */
