#ifndef PLATFORM_HOST_FE_TRANSITION_H
#define PLATFORM_HOST_FE_TRANSITION_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_fe_transition_create(const char* asset_dir);
void host_fe_transition_destroy(void* self);

void cFEStateTransition_onCreateScreen(void* self);
void cFEStateTransition_onDestroyScreen(void* self);
void cFEStateTransition_onUpdate(void* self);
void cFEStateTransition_onScreenEvent(void* self, int event);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_TRANSITION_H */
