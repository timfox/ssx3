#include "platform/host_fe_transition.h"

#include "platform/host_fe_transition_state.h"
#include "platform/host_visualfx_menu.h"

#include <cstring>

namespace host {

HostFETransition* fe_transition_as_state(void* self) {
    auto* tr = static_cast<HostFETransition*>(self);
    if (!tr || tr->magic != kFETransitionMagic) {
        return nullptr;
    }
    return tr;
}

} // namespace host

extern "C" {

void* host_fe_transition_create(const char* asset_dir) {
    auto* tr = new host::HostFETransition();
    if (asset_dir) {
        std::strncpy(tr->asset_dir, asset_dir, sizeof(tr->asset_dir) - 1);
    }
    tr->visual_fx = host::host_visualfx_mainmenu_create();
    return tr;
}

void host_fe_transition_destroy(void* self) {
    auto* tr = host::fe_transition_as_state(self);
    if (!tr) {
        return;
    }
    if (tr->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(tr->visual_fx);
        tr->visual_fx = nullptr;
    }
    delete tr;
}

} // extern "C"
