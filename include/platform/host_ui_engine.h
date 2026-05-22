#ifndef PLATFORM_HOST_UI_ENGINE_H
#define PLATFORM_HOST_UI_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_ui_engine_create(void);
void host_ui_engine_destroy(void* engine);

/* Retail UI engine @ 0x397C40 — host parses IYF .lui for layout probes. */
int cUIEngine_loadFile(void* engine, const char* path);
int cUIEngine_addScreenByHashName(void* engine, unsigned int screen_hash);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_UI_ENGINE_H */
