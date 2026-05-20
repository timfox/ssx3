#ifndef PLATFORM_HOST_GS_H
#define PLATFORM_HOST_GS_H

namespace host {

struct MenuFrame;
class Renderer;

/* Vulkan renderer registers here; game graphics stubs call gs_submit_menu_frame. */
void gs_set_renderer(Renderer* renderer);
void gs_submit_menu_frame(const MenuFrame& frame);

} // namespace host

#endif /* PLATFORM_HOST_GS_H */
