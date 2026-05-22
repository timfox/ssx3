#ifndef PLATFORM_HOST_RIDER_H
#define PLATFORM_HOST_RIDER_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_rider_create(void);
void host_rider_destroy(void* self);
void cRider_initOnce(void* self);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_RIDER_H */
