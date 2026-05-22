#ifndef PLATFORM_HOST_SYNCTASK_H
#define PLATFORM_HOST_SYNCTASK_H

#include "retail/synctask.h"

namespace host {

constexpr int kSyncTaskPoolCap = kRetailSyncTaskSlots;

void host_synctask_init(void);

} // namespace host

#endif /* PLATFORM_HOST_SYNCTASK_H */
