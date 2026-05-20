#ifndef PLATFORM_HOST_FE_NAV_H
#define PLATFORM_HOST_FE_NAV_H

namespace host {

int fe_mainmenu_item_count();
int fe_mainmenu_get_selected(void* mainmenu);
void fe_mainmenu_set_selected(void* mainmenu, int index);
void fe_mainmenu_navigate(void* mainmenu, int delta);

int fe_mountain_room_peak_count();
int fe_mountain_room_get_peak(void* room);
void fe_mountain_room_set_peak(void* room, int index);
void fe_mountain_room_navigate_peak(void* room, int delta);

int fe_options_item_count();
int fe_options_get_selected(void* options);
void fe_options_navigate(void* options, int delta);
bool fe_options_selection_is_back(void* options);

int fe_options_game_row_count();
int fe_options_game_get_selected(void* game);
void fe_options_game_navigate(void* game, int delta);
bool fe_options_game_selection_is_back(void* game);
void fe_options_game_adjust_value(void* game, int delta);

int fe_options_sound_row_count();
int fe_options_sound_get_selected(void* sound);
void fe_options_sound_navigate(void* sound, int delta);
bool fe_options_sound_selection_is_back(void* sound);
void fe_options_sound_adjust_value(void* sound, int delta);

} // namespace host

#endif /* PLATFORM_HOST_FE_NAV_H */
