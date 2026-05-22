#include "platform/host_fe_options_sound.h"

#include "platform/host_fe_options_sound_state.h"
#include "platform/host_visualfx_menu.h"

#include <algorithm>
#include <cstring>

namespace host {

HostFEStateOptionsSound* fe_options_sound_as_state(void* self) {
    auto* os = static_cast<HostFEStateOptionsSound*>(self);
    if (!os || os->magic != kFEOptionsSoundMagic) {
        return nullptr;
    }
    return os;
}

int fe_options_sound_row_count() { return kSoundOptionRows; }

int fe_options_sound_get_selected(void* self) {
    auto* os = fe_options_sound_as_state(self);
    return os ? os->selected : 0;
}

void fe_options_sound_set_selected(void* self, int index) {
    auto* os = fe_options_sound_as_state(self);
    if (!os) {
        return;
    }
    os->selected = std::clamp(index, 0, kSoundOptionRows - 1);
}

void fe_options_sound_navigate(void* self, int delta) {
    fe_options_sound_set_selected(self, fe_options_sound_get_selected(self) + delta);
}

bool fe_options_sound_selection_is_back(void* self) {
    return fe_options_sound_get_selected(self) == kSoundOptionRows - 1;
}

void fe_options_sound_adjust_value(void* self, int delta) {
    auto* os = fe_options_sound_as_state(self);
    if (!os || os->selected >= kSoundOptionRows - 1) {
        return;
    }
    int& v = os->values[os->selected];
    if (os->selected == 0) {
        v = std::clamp(v + delta, 0, 10);
    } else {
        v = (v + delta) & 1;
    }
}

} // namespace host

extern "C" {

void* host_fe_options_sound_create(const char* asset_dir) {
    auto* os = new host::HostFEStateOptionsSound();
    if (asset_dir) {
        std::strncpy(os->asset_dir, asset_dir, sizeof(os->asset_dir) - 1);
    }
    os->visual_fx = host::host_visualfx_mainmenu_create();
    return os;
}

void host_fe_options_sound_destroy(void* self) {
    auto* os = host::fe_options_sound_as_state(self);
    if (!os) {
        return;
    }
    if (os->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(os->visual_fx);
        os->visual_fx = nullptr;
    }
    delete os;
}

} // extern "C"
