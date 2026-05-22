#include "platform/host_retail.h"

#include "platform/host_abi.h"
#include "platform/host_log.h"
#include "visualfx/crowdrender2d.h"

extern "C" void SYNCTASK_init(void);
extern "C" void cRider_initOnce(void* self);

namespace host {
namespace {

constexpr int kModuleCount = 12;

} // namespace
} // namespace host

extern "C" {

void host_retail_module_init(void* app_base, int module_index) {
    (void)app_base;
    if (module_index < 0 || module_index >= host::kModuleCount) {
        return;
    }

    switch (module_index) {
    case 0:
        /* cMemMan_initialize — already invoked from systemInit in boot chain. */
        host::host_log("module", "init[0] memory manager (systemInit)");
        break;
    case 1:
        cCrowdRender2D_init();
        host::host_log("module", "init[1] cCrowdRender2D_init (decompiled)");
        break;
    case 2:
        SYNCTASK_init();
        host::host_log("module", "init[2] SYNCTASK_init");
        break;
    case 3:
        host::host_log("module", "init[3] input / pad HAL");
        break;
    case 4:
        host::host_log("module", "init[4] FILESYS / BIG HAL");
        break;
    case 5:
        host::host_log("module", "init[5] graphics HAL (Vulkan)");
        break;
    case 6:
        host::host_log("module", "init[6] audio HAL (stub)");
        break;
    case 7:
        host::host_log("module", "init[7] FE / UI engine HAL");
        break;
    case 8:
        host::host_log("module", "init[8] world / stream (stub)");
        break;
    case 9:
        cRider_initOnce(app_base);
        host::host_log("module", "init[9] cRider_initOnce (decompiled)");
        break;
    case 10:
        host::host_log("module", "init[10] career / save (stub)");
        break;
    case 11:
        host::host_log("module", "init[11] online (stub)");
        break;
    default:
        break;
    }
}

} // extern "C"
