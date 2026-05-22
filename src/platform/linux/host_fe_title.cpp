#include "platform/host_fe_title.h"

#include "platform/host_fe_title_state.h"
#include "platform/host_fe_session.h"
#include "platform/host_log.h"
#include "platform/host_visualfx_menu.h"

#include <cstring>

namespace host {

HostFEStateTitle* fe_title_as_state(void* self) {
    auto* fe = static_cast<HostFEStateTitle*>(self);
    if (!fe || fe->magic != kFEStateTitleMagic) {
        return nullptr;
    }
    return fe;
}

void* host_fe_title_create(const char* asset_dir) {
    auto* fe = new HostFEStateTitle();
    if (asset_dir) {
        std::strncpy(fe->asset_dir, asset_dir, sizeof(fe->asset_dir) - 1);
    }
    fe->visual_fx = host_visualfx_mainmenu_create();
    return fe;
}

void host_fe_title_destroy(void* self) {
    auto* fe = fe_title_as_state(self);
    if (!fe) {
        return;
    }
    if (fe->visual_fx) {
        host_visualfx_mainmenu_set_active(nullptr);
        host_visualfx_mainmenu_destroy(fe->visual_fx);
        fe->visual_fx = nullptr;
    }
    delete fe;
}

bool run_fe_title_menu_loop(Renderer* renderer, const std::string& asset_dir) {
    return run_fe_menu_session(renderer, asset_dir);
}

} // namespace host
