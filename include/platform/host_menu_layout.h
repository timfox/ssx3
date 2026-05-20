#ifndef PLATFORM_HOST_MENU_LAYOUT_H
#define PLATFORM_HOST_MENU_LAYOUT_H

#include "platform/host_menu_frame.h" // MenuFrame, MenuViewport, MenuSprite

namespace host {

// NTSC framebuffer space used by the PS2 front-end (4:3).
inline constexpr int kTitleVirtualWidth = 640;
inline constexpr int kTitleVirtualHeight = 448;

MenuViewport compute_title_viewport(int window_width, int window_height);

void build_title_screen(MenuFrame& frame, const MenuViewport& viewport, float time_sec, float dt);

void reset_title_screen_state();

} // namespace host

#endif /* PLATFORM_HOST_MENU_LAYOUT_H */
