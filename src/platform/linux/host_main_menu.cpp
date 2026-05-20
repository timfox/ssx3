#include "platform/host_main_menu.h"

#include "platform/host_fe_session.h"

namespace host {

bool run_main_menu_loop(Renderer* renderer, const std::string& asset_dir) {
    return run_fe_menu_session(renderer, asset_dir);
}

} // namespace host
