#ifndef PLATFORM_HOST_SSXAPP_H
#define PLATFORM_HOST_SSXAPP_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_ssxapp_create(void);
void host_ssxapp_destroy(void* self);

void cSSXApp_cSSXApp(void* self);
void cSSXApp__cSSXApp(void* self);
int cSSXApp_init(void* self);
void cSSXApp_purge(void* self);
void cSSXApp_preUpdate(void* self);
void cSSXApp_flush(void* self);
int cSSXApp_initLocale(void* self);
int cSSXApp_parseCommandLine(void* self, int argc, char** argv);
void cSSXApp_timerCallback(void* self);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_SSXAPP_H */
