#ifndef PLATFORM_HOST_FE_FLOW_H
#define PLATFORM_HOST_FE_FLOW_H

namespace host {

enum class FEScreen {
    Title,
    MainMenu,
    MountainRoom,
    Options,
    OptionsGame,
};

enum class FEMainMenuAction {
    None,
    MountainRoom,
    Options,
    Stub,
};

enum class FEOptionsAction {
    Back,
    OptionsGame,
    Stub,
};

const char* fe_mainmenu_item_label(int index);
FEMainMenuAction fe_mainmenu_action_for_index(int index);

const char* fe_options_item_label(int index);
FEOptionsAction fe_options_action_for_index(int index);

} // namespace host

#endif /* PLATFORM_HOST_FE_FLOW_H */
