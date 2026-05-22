#include "platform/host_fe_options.h"

#include "platform/host_fe_options_state.h"
#include "platform/host_fe_strings.h"
#include "platform/host_visualfx_menu.h"

#include <cstring>

namespace host {

HostFEStateOptions* fe_options_as_state(void* self) {
    auto* opt = static_cast<HostFEStateOptions*>(self);
    if (!opt || opt->magic != kFEOptionsMagic) {
        return nullptr;
    }
    return opt;
}

int fe_options_get_selected(void* self) {
    auto* opt = fe_options_as_state(self);
    return opt ? opt->selected : 0;
}

void fe_options_navigate(void* self, int delta) {
    auto* opt = fe_options_as_state(self);
    if (!opt) {
        return;
    }
    int next = opt->selected + delta;
    if (next < 0) {
        next = 0;
    }
    if (next >= fe_options_item_count()) {
        next = fe_options_item_count() - 1;
    }
    opt->selected = next;
}

bool fe_options_selection_is_back(void* self) {
    return fe_options_get_selected(self) == fe_options_item_count() - 1;
}

} // namespace host

extern "C" {

void* host_fe_options_create(const char* asset_dir) {
    auto* opt = new host::HostFEStateOptions();
    if (asset_dir) {
        std::strncpy(opt->asset_dir, asset_dir, sizeof(opt->asset_dir) - 1);
    }
    opt->visual_fx = host::host_visualfx_mainmenu_create();
    return opt;
}

void host_fe_options_destroy(void* self) {
    auto* opt = host::fe_options_as_state(self);
    if (!opt) {
        return;
    }
    if (opt->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(opt->visual_fx);
        opt->visual_fx = nullptr;
    }
    delete opt;
}

} // extern "C"
