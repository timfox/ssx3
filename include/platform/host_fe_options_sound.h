#ifndef PLATFORM_HOST_FE_OPTIONS_SOUND_H
#define PLATFORM_HOST_FE_OPTIONS_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_fe_options_sound_create(const char* asset_dir);
void host_fe_options_sound_destroy(void* self);

void cFEStateOptionsSound_onCreateScreen(void* self);
void cFEStateOptionsSound_onDestroyScreen(void* self);
void cFEStateOptionsSound_onUpdate(void* self);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_OPTIONS_SOUND_H */
