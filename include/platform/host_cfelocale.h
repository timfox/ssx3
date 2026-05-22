#ifndef PLATFORM_HOST_CFELOCALE_H
#define PLATFORM_HOST_CFELOCALE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Retail cFELocale_addFile @ 0x1959A8 — host parses LOCH .loc from disc/. */
int cFELocale_addFile(void* locale, const char* path);

/* Retail string pool lookup by tHashName32 id (host maps loaded .loc strings). */
const char* cFELocale_getString(void* locale, unsigned int hash_id);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_CFELOCALE_H */
