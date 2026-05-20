#ifndef PLATFORM_HOST_MENU_LAYOUT_H
#define PLATFORM_HOST_MENU_LAYOUT_H

#include "platform/host_menu_frame.h"

namespace host {

// NTSC framebuffer space used by the PS2 front-end (4:3).
inline constexpr int kTitleVirtualWidth = 640;
inline constexpr int kTitleVirtualHeight = 448;

MenuViewport compute_title_viewport(int window_width, int window_height);

void build_title_screen(MenuFrame& frame,
                        const MenuViewport& viewport,
                        float time_sec,
                        float dt,
                        const MenuRetailStyle& style = MenuRetailStyle{});

void reset_title_screen_state();

void build_main_menu_screen(MenuFrame& frame,
                            const MenuViewport& viewport,
                            float time_sec,
                            int selected_item,
                            int item_count);

void build_mountain_room_screen(MenuFrame& frame,
                                const MenuViewport& viewport,
                                float time_sec,
                                int selected_peak,
                                int peak_count);

void build_options_screen(MenuFrame& frame,
                          const MenuViewport& viewport,
                          float time_sec,
                          int selected_item,
                          int item_count);

void build_options_game_screen(MenuFrame& frame,
                               const MenuViewport& viewport,
                               float time_sec,
                               int selected_row,
                               int row_count);

} // namespace host

#endif /* PLATFORM_HOST_MENU_LAYOUT_H */
