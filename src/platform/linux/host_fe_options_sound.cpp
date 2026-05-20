#include "platform/host_fe_options_sound.h"

#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <algorithm>
#include <cstdint>
#include <cstring>

namespace host {

constexpr std::uint32_t kOptionsSoundMagic = 0x46454F53u; /* "FEOS" */
constexpr int kSoundOptionRows = 5;

struct HostFEStateOptionsSound {
    std::uint32_t magic = kOptionsSoundMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int values[kSoundOptionRows] = {8, 1, 1, 1, 0};
    int initialized = 0;
};

HostFEStateOptionsSound* as_os(void* self) {
    auto* os = static_cast<HostFEStateOptionsSound*>(self);
    if (!os || os->magic != kOptionsSoundMagic) {
        return nullptr;
    }
    return os;
}

int fe_options_sound_row_count() { return kSoundOptionRows; }

int fe_options_sound_get_selected(void* self) {
    auto* os = as_os(self);
    return os ? os->selected : 0;
}

void fe_options_sound_set_selected(void* self, int index) {
    auto* os = as_os(self);
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
    auto* os = as_os(self);
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
    auto* os = host::as_os(self);
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

void cFEStateOptionsSound_onCreateScreen(void* self) {
    auto* os = host::as_os(self);
    if (!os) {
        return;
    }
    host::host_log("fe", "cFEStateOptionsSound_onCreateScreen (retail 0x18A298)");

    if (!os->visual_fx) {
        os->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(os->visual_fx, nullptr);
    os->selected = 0;
    os->initialized = 1;
}

void cFEStateOptionsSound_onDestroyScreen(void* self) {
    auto* os = host::as_os(self);
    if (!os) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    os->initialized = 0;
    host::host_log("fe", "cFEStateOptionsSound_onDestroyScreen");
}

void cFEStateOptionsSound_onUpdate(void* self) {
    auto* os = host::as_os(self);
    if (!os || !os->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    os->frame.time_sec += host::fe_frame_dt();
    host::build_options_sound_screen(os->frame,
                                     viewport,
                                     os->frame.time_sec,
                                     os->selected,
                                     host::kSoundOptionRows,
                                     os->values,
                                     host::kSoundOptionRows);
    cVisualEffectsMainMenu_render(os->visual_fx, &os->frame);
}

} // extern "C"
