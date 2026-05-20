#include "platform/host_log.h"

namespace host {
} // namespace host

extern "C" {

void FILESYS_bypassqueuefileinfo(void* a0, void* a1, void* a2) {
    (void)a0;
    (void)a1;
    (void)a2;
    host::host_stub_log("FILESYS_bypassqueuefileinfo");
}

void FILESYS_atomic(void) { host::host_stub_log("FILESYS_atomic"); }

void iFILESYS_ExecCommand(void) { host::host_stub_log("iFILESYS_ExecCommand"); }

void iFILESYS_CommandCompleteCallback(void) { host::host_stub_log("iFILESYS_CommandCompleteCallback"); }

} // extern "C"
