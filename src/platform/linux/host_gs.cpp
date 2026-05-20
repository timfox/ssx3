#include "platform/host_gs.h"

#include "platform/host_menu_frame.h"
#include "platform/host_renderer.h"

namespace host {
namespace {

Renderer* g_renderer = nullptr;

} // namespace

void gs_set_renderer(Renderer* renderer) {
    g_renderer = renderer;
}

void gs_submit_menu_frame(const MenuFrame& frame) {
    if (g_renderer) {
        g_renderer->set_main_menu_frame(frame);
    }
}

} // namespace host
