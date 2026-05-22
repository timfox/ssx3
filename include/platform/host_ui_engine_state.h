#ifndef PLATFORM_HOST_UI_ENGINE_STATE_H
#define PLATFORM_HOST_UI_ENGINE_STATE_H

#include <cstdint>

namespace host {

constexpr std::uint32_t kUIEngineMagic = 0x55494547u; /* "UIEG" */

struct HostUIEngine {
    std::uint32_t magic = kUIEngineMagic;
    int last_widget_count = 0;
    char last_path[260]{};
};

HostUIEngine* fe_ui_engine_as_state(void* engine);

} // namespace host

#endif /* PLATFORM_HOST_UI_ENGINE_STATE_H */
