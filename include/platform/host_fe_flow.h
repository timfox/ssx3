#ifndef PLATFORM_HOST_FE_FLOW_H
#define PLATFORM_HOST_FE_FLOW_H

namespace host {

enum class FEScreen {
    Title,
    MainMenu,
    MountainRoom,
    Options,
    OptionsGame,
    OptionsSound,
    Previews,
    MultiPlay,
    SingleEvent,
    PeakRoom,
    Message,
};

enum class FEMainMenuAction {
    None,
    MountainRoom,
    Options,
    SingleEvent,
    MultiPlay,
    Previews,
    Online,
    Stub,
};

enum class FEOptionsAction {
    Back,
    OptionsGame,
    OptionsSound,
    OptionsController,
    OptionsSaveLoad,
    Stub,
};

const char* fe_mainmenu_item_label(int index);
FEMainMenuAction fe_mainmenu_action_for_index(int index);

const char* fe_options_item_label(int index);
FEOptionsAction fe_options_action_for_index(int index);

} // namespace host

#endif /* PLATFORM_HOST_FE_FLOW_H */
