#ifndef PLATFORM_HOST_FE_LOCALE_H
#define PLATFORM_HOST_FE_LOCALE_H

namespace host {

/** Load curated retail strings from assets/host/locale/fe_strings.json. */
bool fe_locale_init();

/** Lookup by key (e.g. fe.main_menu.single_event); nullptr if missing. */
const char* fe_locale_get(const char* key);

/** Lookup retail NTSC string body loaded from a .loc pool (exact match). */
const char* fe_locale_by_text(const char* text);

/** Lookup by EA tHashName32 (retail widget / loc ids). */
const char* fe_locale_by_hash(unsigned int hash);

/** Load UTF-16 strings from disc path (PS2 or host-relative). Returns count added. */
int fe_locale_add_loc_file(const char* path);

} // namespace host

#endif /* PLATFORM_HOST_FE_LOCALE_H */
