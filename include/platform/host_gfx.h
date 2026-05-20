#ifndef PLATFORM_HOST_GFX_H
#define PLATFORM_HOST_GFX_H

#include "platform/host_renderer.h"

#include <string>

namespace host {

bool run_vulkan_demo(const RendererConfig& config);

bool run_native_gfx_session(const RendererConfig& config,
                            const std::string& disc_root,
                            bool boot_videos,
                            bool run_demo_after = true);

} // namespace host

#endif /* PLATFORM_HOST_GFX_H */
