#ifndef PLATFORM_HOST_H
#define PLATFORM_HOST_H

/* Native Linux build: compile game sources with -DSSX3_HOST -DSKIP_ASM. */
#ifdef SSX3_HOST

#include <cstddef>
#include <cstdint>

using uint = std::uint32_t;
using ulong = std::uint64_t;

#endif /* SSX3_HOST */

#endif /* PLATFORM_HOST_H */
