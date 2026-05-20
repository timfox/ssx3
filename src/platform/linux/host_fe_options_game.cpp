#include "platform/host_fe_options_game.h"

#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <algorithm>
#include <cstdint>
#include <cstring>

namespace host {

constexpr std::uint32_t kOptionsGameMagic = 0x46454F47u; /* "FEOG" */
constexpr int kGameOptionRows = 5;

struct HostFEStateOptionsGame {
    std::uint32_t magic = kOptionsGameMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int values[kGameOptionRows] = {1, 0, 1, 1, 0};
    int initialized = 0;
};

constexpr int kGameValueCounts[] = {3, 3, 3, 2, 1};

HostFEStateOptionsGame* as_og(void* self) {
    auto* og = static_cast<HostFEStateOptionsGame*>(self);
    if (!og || og->magic != kOptionsGameMagic) {
        return nullptr;
    }
    return og;
}

int fe_options_game_row_count() { return kGameOptionRows; }

int fe_options_game_get_selected(void* self) {
    auto* og = as_og(self);
    return og ? og->selected : 0;
}

void fe_options_game_set_selected(void* self, int index) {
    auto* og = as_og(self);
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
    auto* og = as_og(self);
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
    auto* og = host::as_og(self);
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

void cFEStateOptionsGame_onCreateScreen(void* self) {
    auto* og = host::as_og(self);
    if (!og) {
        return;
    }
    host::host_log("fe", "cFEStateOptionsGame_onCreateScreen (retail 0x188F28)");

    if (!og->visual_fx) {
        og->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(og->visual_fx, nullptr);
    og->selected = 0;
    og->initialized = 1;
}

void cFEStateOptionsGame_onDestroyScreen(void* self) {
    auto* og = host::as_og(self);
    if (!og) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    og->initialized = 0;
    host::host_log("fe", "cFEStateOptionsGame_onDestroyScreen");
}

void cFEStateOptionsGame_onUpdate(void* self) {
    auto* og = host::as_og(self);
    if (!og || !og->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    og->frame.time_sec += host::fe_frame_dt();
    host::build_options_game_screen(og->frame,
                                    viewport,
                                    og->frame.time_sec,
                                    og->selected,
                                    host::kGameOptionRows,
                                    og->values,
                                    host::kGameOptionRows);
    cVisualEffectsMainMenu_render(og->visual_fx, &og->frame);
}

void cFEStateOptionsGame_onWidgetEvent(void* self, void* widget, int event) {
    (void)widget;
    (void)event;
    host::host_log("fe", "cFEStateOptionsGame_onWidgetEvent (retail 0x189980, stub)");
    (void)self;
}

} // extern "C"
