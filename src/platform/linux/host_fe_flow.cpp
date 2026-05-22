#include "platform/host_fe_flow.h"

#include "platform/host_fe_strings.h"

namespace host {

const char* fe_mainmenu_item_label(int index) {
    return fe_mainmenu_label(index);
}

FEMainMenuAction fe_mainmenu_action_for_index(int index) {
    switch (index) {
    case 0:
        return FEMainMenuAction::SingleEvent;
    case 1:
        return FEMainMenuAction::MountainRoom;
    case 2:
        return FEMainMenuAction::MultiPlay;
    case 3:
        return FEMainMenuAction::Previews;
    case 4:
        return FEMainMenuAction::Online;
    default:
        return FEMainMenuAction::Stub;
    }
}

const char* fe_options_item_label(int index) {
    return fe_options_label(index);
}

FEOptionsAction fe_options_action_for_index(int index) {
    switch (index) {
    case 0:
        return FEOptionsAction::OptionsGame;
    case 1:
        return FEOptionsAction::OptionsSound;
    case 2:
        return FEOptionsAction::OptionsController;
    case 3:
        return FEOptionsAction::OptionsSaveLoad;
    case 4:
        return FEOptionsAction::Back;
    default:
        return FEOptionsAction::Stub;
    }
}

} // namespace host
