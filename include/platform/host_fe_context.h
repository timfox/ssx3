#ifndef PLATFORM_HOST_FE_CONTEXT_H
#define PLATFORM_HOST_FE_CONTEXT_H

namespace host {

void fe_set_frame_context(int window_width, int window_height, float dt_sec);
int fe_frame_width();
int fe_frame_height();
float fe_frame_dt();

} // namespace host

#endif /* PLATFORM_HOST_FE_CONTEXT_H */
