#include "platform/host_fe_options_game.h"

#include "platform/host_fe_options_game_state.h"
#include "platform/host_visualfx_menu.h"

#include <algorithm>
#include <cstring>

namespace host {

constexpr int kGameValueCounts[] = {3, 3, 3, 2, 1};

HostFEStateOptionsGame* fe_options_game_as_state(void* self) {
    auto* og = static_cast<HostFEStateOptionsGame*>(self);
    if (!og || og->magic != kFEOptionsGameMagic) {
        return nullptr;
    }
    return og;
}

int fe_options_game_row_count() { return kGameOptionRows; }

int fe_options_game_get_selected(void* self) {
    auto* og = fe_options_game_as_state(self);
    return og ? og->selected : 0;
}

void fe_options_game_set_selected(void* self, int index) {
    auto* og = fe_options_game_as_state(self);
    if (!og) {
        return;
    }
    if (index < 0) {
        index = 0;
    }
    if (index >= kGameOptionRows) {
        index = kGameOptionRows - 1;
    }
    og->selected = index;
}

void fe_options_game_navigate(void* self, int delta) {
    fe_options_game_set_selected(self, fe_options_game_get_selected(self) + delta);
}

bool fe_options_game_selection_is_back(void* self) {
    return fe_options_game_get_selected(self) == kGameOptionRows - 1;
}

void fe_options_game_adjust_value(void* self, int delta) {
    auto* og = fe_options_game_as_state(self);
    if (!og || og->selected >= kGameOptionRows - 1) {
        return;
    }
    const int max_v = kGameValueCounts[og->selected];
    int& v = og->values[og->selected];
    v = (v + delta) % max_v;
    if (v < 0) {
        v += max_v;
    }
}

} // namespace host

extern "C" {

void* host_fe_options_game_create(const char* asset_dir) {
    auto* og = new host::HostFEStateOptionsGame();
    if (asset_dir) {
        std::strncpy(og->asset_dir, asset_dir, sizeof(og->asset_dir) - 1);
    }
    og->visual_fx = host::host_visualfx_mainmenu_create();
    return og;
}

void host_fe_options_game_destroy(void* self) {
    auto* og = host::fe_options_game_as_state(self);
    if (!og) {
        return;
    }
    if (og->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(og->visual_fx);
        og->visual_fx = nullptr;
    }
    delete og;
}

} // extern "C"
