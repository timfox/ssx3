#ifndef PLATFORM_HOST_FE_SESSION_H
#define PLATFORM_HOST_FE_SESSION_H

#include "platform/host_renderer.h"

#include <string>

namespace host {

/* Title screen → main menu (retail FE state names, host HAL bodies). */
bool run_fe_menu_session(Renderer* renderer, const std::string& asset_dir);

} // namespace host

#endif /* PLATFORM_HOST_FE_SESSION_H */
