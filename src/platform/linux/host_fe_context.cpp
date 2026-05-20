#include "platform/host_fe_context.h"

namespace host {
namespace {

int g_frame_w = 0;
int g_frame_h = 0;
float g_frame_dt = 0.0f;

} // namespace

void fe_set_frame_context(int window_width, int window_height, float dt_sec) {
    g_frame_w = window_width;
    g_frame_h = window_height;
    g_frame_dt = dt_sec;
}

int fe_frame_width() { return g_frame_w; }
int fe_frame_height() { return g_frame_h; }
float fe_frame_dt() { return g_frame_dt; }

} // namespace host
