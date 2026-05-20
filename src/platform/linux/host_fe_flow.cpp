#include "platform/host_fe_flow.h"

namespace host {

const char* fe_mainmenu_item_label(int index) {
    static const char* kLabels[] = {
        "Freeride",
        "World Tour",
        "Multiplayer",
        "Options",
        "Profile",
    };
    const int count = static_cast<int>(sizeof(kLabels) / sizeof(kLabels[0]));
    if (index < 0 || index >= count) {
        return "?";
    }
    return kLabels[index];
}

FEMainMenuAction fe_mainmenu_action_for_index(int index) {
    switch (index) {
    case 0:
        return FEMainMenuAction::MountainRoom;
    case 3:
        return FEMainMenuAction::Options;
    default:
        return FEMainMenuAction::Stub;
    }
}

const char* fe_options_item_label(int index) {
    static const char* kLabels[] = {
        "Game",
        "Sound",
        "Controller",
        "Save / Load",
        "Back",
    };
    const int count = static_cast<int>(sizeof(kLabels) / sizeof(kLabels[0]));
    if (index < 0 || index >= count) {
        return "?";
    }
    return kLabels[index];
}

FEOptionsAction fe_options_action_for_index(int index) {
    switch (index) {
    case 0:
        return FEOptionsAction::OptionsGame;
    case 1:
        return FEOptionsAction::OptionsSound;
    case 4:
        return FEOptionsAction::Back;
    default:
        return FEOptionsAction::Stub;
    }
}

} // namespace host
