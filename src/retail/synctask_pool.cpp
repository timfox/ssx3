// Native synctask — retail layout: 16 slots @ D_0051ED98, live count @ D_00450e00.

#include "retail/synctask.h"
#include "retail/synctask_retail.h"

#include "platform/host_synctask.h"

#include <cstdint>

namespace {

RetailSyncTaskSlot g_pool[kRetailSyncTaskSlots];
int g_live_count = 0;
int g_chain_head = 0;

extern "C" int retail_synctask_add_slots(
    RetailSyncTaskSlot* pool,
    int* live_count,
    int* chain_head,
    void* task_desc,
    void* aux,
    int chain_delta);

extern "C" void retail_synctask_del_slots(RetailSyncTaskSlot* pool, void* task_desc);

extern "C" void retail_synctask_run_slots(RetailSyncTaskSlot* pool, void* tick_ctx);

} // namespace

extern "C" {

int SYNCTASK_add(void* task_desc) {
    auto* desc = static_cast<RetailSyncTaskDesc*>(task_desc);
    if (!desc || !desc->field0 || !desc->field4) {
        return -1;
    }
    return retail_synctask_add_slots(
        g_pool, &g_live_count, &g_chain_head, task_desc, (void*)(intptr_t)-1, 0);
}

int retail_synctask_add(void* task_desc, void* aux, int chain_delta) {
    return retail_synctask_add_slots(
        g_pool, &g_live_count, &g_chain_head, task_desc, aux, chain_delta);
}

void SYNCTASK_del(void* task_desc) {
    retail_synctask_del_slots(g_pool, task_desc);
}

void retail_synctask_del(void* task_desc) {
    retail_synctask_del_slots(g_pool, task_desc);
}

void SYNCTASK_run(void) {
    retail_synctask_run_slots(g_pool, nullptr);
}

void retail_synctask_run(void* tick_ctx) {
    retail_synctask_run_slots(g_pool, tick_ctx);
}

} // extern "C"

namespace host {

void host_synctask_init(void) {
    retail_memset_u8(g_pool, 0, kRetailSyncTaskPoolBytes);
    g_live_count = 0;
    g_chain_head = 0;
}

} // namespace host
