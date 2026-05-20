#ifndef PLATFORM_HOST_FE_OPTIONS_H
#define PLATFORM_HOST_FE_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_fe_options_create(const char* asset_dir);
void host_fe_options_destroy(void* self);

void cFEStateOptions_onCreateScreen(void* self);
void cFEStateOptions_onDestroyScreen(void* self);
void cFEStateOptions_onUpdate(void* self);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_OPTIONS_H */
