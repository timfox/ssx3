// Retail cSectionMan_setSky @ 0x22E180 — native stub until sectionman.cpp is decompiled.

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_log.h"

#include <cstdio>

extern "C" {
#endif

void cSectionMan_setSky(int section, int sky_id) {
    char log_buf[80];
    std::snprintf(log_buf, sizeof(log_buf), "cSectionMan_setSky section=%d sky=%d", section, sky_id);
    host::host_log("world", log_buf);
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
