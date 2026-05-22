#ifndef PLATFORM_HOST_FE_STRINGS_H
#define PLATFORM_HOST_FE_STRINGS_H

namespace host {

/* Confirmed in SLUS_207.72 / retail FE tables. */
const char* fe_string_title_press();
const char* fe_string_title_copyright();
const char* fe_string_main_menu_title();

int fe_mainmenu_item_count();
const char* fe_mainmenu_label(int index);
const char* fe_mainmenu_description(int index);
const char* fe_string_main_menu_hints();
const char* fe_string_hint_select();
const char* fe_string_hint_previous();
const char* fe_string_hint_options();

int fe_options_item_count();
const char* fe_options_label(int index);

int fe_mountain_peak_count();
const char* fe_mountain_peak_label(int index);
const char* fe_string_mountain_select_peak();

const char* fe_previews_screen_title();
int fe_previews_item_count();
const char* fe_previews_label(int index);
const char* fe_previews_description(int index);

const char* fe_multiplay_screen_title();
int fe_multiplay_item_count();
const char* fe_multiplay_label(int index);
const char* fe_multiplay_description(int index);

const char* fe_single_event_screen_title();
int fe_single_event_item_count();
const char* fe_single_event_label(int index);
const char* fe_single_event_description(int index);

int fe_peak_room_item_count();
const char* fe_peak_room_label(int index);
const char* fe_peak_room_description(int index);

const char* fe_online_screen_title();
const char* fe_online_body_text();

const char* fe_options_controller_body();
const char* fe_options_saveload_body();

} // namespace host

#endif /* PLATFORM_HOST_FE_STRINGS_H */
