#ifndef PLATFORM_HOST_BIGFILE_H
#define PLATFORM_HOST_BIGFILE_H

#include <cstdint>

#ifdef SSX3_HOST
#include "platform/host.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Retail cBigFile layout is larger; this is the host-minimal prefix used by HAL open/close.
 * Expand when bx/bigfile.cpp is decompiled and linked.
 */
struct HostBigFile {
    std::uint32_t magic;
    void* archive;
    char path[260];
};

void cBigFile_cBigFile(HostBigFile* self);
void cBigFile__cBigFile(HostBigFile* self);
int cBigFile_open(HostBigFile* self, const char* path);
void cBigFile_close(HostBigFile* self);

/* Read an entry from an opened BIG archive (returns bytes read, 0 on failure). */
unsigned long cBigFile_read(HostBigFile* self, const char* entry_name, void* dst, unsigned long dst_size);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_BIGFILE_H */
