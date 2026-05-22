#ifndef PLATFORM_HOST_RETAIL_H
#define PLATFORM_HOST_RETAIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Mirrors retail main() module loop (0x31B098) — one init hook per index. */
void host_retail_module_init(void* app_base, int module_index);

/* Per-frame services game code expects between updates. */
void host_retail_tick(void* ssx_app);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_RETAIL_H */
