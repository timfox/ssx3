#ifndef PLATFORM_HOST_GAMEPLAY_H
#define PLATFORM_HOST_GAMEPLAY_H

#include "platform/host_renderer.h"

namespace host {

bool run_world_gameplay_session(Renderer* renderer, void* ssx_app = nullptr);

bool run_native_world_session(const RendererConfig& config, bool retail_boot_init = false);

/** cAppMan_mainLoop with Vulkan presentation (replaces SDL-only window). */
bool run_boot_appman_vulkan_session(void* ssx_app);

} // namespace host

#endif /* PLATFORM_HOST_GAMEPLAY_H */
