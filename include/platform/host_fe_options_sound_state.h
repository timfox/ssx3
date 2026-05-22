#ifndef PLATFORM_HOST_FE_OPTIONS_SOUND_STATE_H
#define PLATFORM_HOST_FE_OPTIONS_SOUND_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEOptionsSoundMagic = 0x46454F53u; /* "FEOS" */
constexpr int kSoundOptionRows = 5;

struct HostFEStateOptionsSound {
    std::uint32_t magic = kFEOptionsSoundMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int values[kSoundOptionRows] = {8, 1, 1, 1, 0};
    int initialized = 0;
};

HostFEStateOptionsSound* fe_options_sound_as_state(void* self);
int fe_options_sound_row_count();
int fe_options_sound_get_selected(void* self);
void fe_options_sound_set_selected(void* self, int index);
void fe_options_sound_navigate(void* self, int delta);
bool fe_options_sound_selection_is_back(void* self);
void fe_options_sound_adjust_value(void* self, int delta);

} // namespace host

#endif /* PLATFORM_HOST_FE_OPTIONS_SOUND_STATE_H */
