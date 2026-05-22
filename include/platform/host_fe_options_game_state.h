#ifndef PLATFORM_HOST_FE_OPTIONS_GAME_STATE_H
#define PLATFORM_HOST_FE_OPTIONS_GAME_STATE_H

#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>

namespace host {

constexpr std::uint32_t kFEOptionsGameMagic = 0x46454F47u; /* "FEOG" */
constexpr int kGameOptionRows = 5;

struct HostFEStateOptionsGame {
    std::uint32_t magic = kFEOptionsGameMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int values[kGameOptionRows] = {1, 0, 1, 1, 0};
    int initialized = 0;
};

HostFEStateOptionsGame* fe_options_game_as_state(void* self);
int fe_options_game_row_count();
int fe_options_game_get_selected(void* self);
void fe_options_game_set_selected(void* self, int index);
void fe_options_game_navigate(void* self, int delta);
bool fe_options_game_selection_is_back(void* self);
void fe_options_game_adjust_value(void* self, int delta);

} // namespace host

#endif /* PLATFORM_HOST_FE_OPTIONS_GAME_STATE_H */
