#ifndef PLATFORM_HOST_VISUALFX_MENU_H
#define PLATFORM_HOST_VISUALFX_MENU_H

#include "platform/host_menu_frame.h"

namespace host {

struct HostVisualEffectsMainMenu;

HostVisualEffectsMainMenu* host_visualfx_mainmenu_create();
void host_visualfx_mainmenu_destroy(HostVisualEffectsMainMenu* fx);

void host_visualfx_mainmenu_reset(HostVisualEffectsMainMenu* fx);
void host_visualfx_mainmenu_set_snow_colour(HostVisualEffectsMainMenu* fx, float r, float g, float b);
float host_visualfx_mainmenu_snow_r(const HostVisualEffectsMainMenu* fx);
float host_visualfx_mainmenu_snow_g(const HostVisualEffectsMainMenu* fx);
float host_visualfx_mainmenu_snow_b(const HostVisualEffectsMainMenu* fx);

void host_visualfx_mainmenu_build_frame(HostVisualEffectsMainMenu* fx,
                                        MenuFrame& frame,
                                        const MenuViewport& viewport,
                                        float time_sec,
                                        float dt);

void host_visualfx_mainmenu_set_active(HostVisualEffectsMainMenu* fx);
HostVisualEffectsMainMenu* host_visualfx_mainmenu_active();

} // namespace host

#endif /* PLATFORM_HOST_VISUALFX_MENU_H */
