#ifndef PLATFORM_HOST_FE_TITLE_H
#define PLATFORM_HOST_FE_TITLE_H

#include "platform/host_renderer.h"

#include <string>

namespace host {

/* Host FE title state (retail cFEStateTitle @ 0x194778). */
void* host_fe_title_create(const char* asset_dir);
void host_fe_title_destroy(void* self);

bool run_fe_title_menu_loop(Renderer* renderer, const std::string& asset_dir);

} // namespace host

#endif /* PLATFORM_HOST_FE_TITLE_H */
