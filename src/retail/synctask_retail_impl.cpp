#include "retail/synctask_retail.h"

#include <stdint.h>

extern "C" {

/* Shared retail-faithful logic (native pool pointers supplied by synctask_pool.cpp). */

int retail_synctask_add_slots(
    RetailSyncTaskSlot* pool,
    int* live_count,
    int* chain_head,
    void* task_desc,
    void* aux,
    int chain_delta) {
    if (!pool || !task_desc) {
        return -1;
    }

    int aux_byte = 0;
    int delta = 0;
    if (aux == (void*)(intptr_t)-1) {
        delta = chain_delta;
    } else {
        aux_byte = 1;
    }

    ++(*live_count);

    int slot_index = -1;
    for (int i = 0; i < kRetailSyncTaskSlots; ++i) {
        void* owner = pool[i].owner;
        if (owner == task_desc) {
            slot_index = i;
            continue;
        }
        if (owner != nullptr) {
            continue;
        }
        slot_index = i;
        break;
    }

    if (slot_index < 0) {
        --(*live_count);
        return -1;
    }

    RetailSyncTaskSlot* slot = &pool[slot_index];
    slot->owner = task_desc;
    slot->aux = (void*)(intptr_t)aux_byte;
    slot->chain = *chain_head + delta;
    slot->flags = 0;
    *chain_head = slot->chain;

    --(*live_count);
    return 1;
}

void retail_synctask_del_slots(RetailSyncTaskSlot* pool, void* task_desc) {
    if (!pool || !task_desc) {
        return;
    }

    if (pool[0].owner == task_desc) {
        pool[0].owner = nullptr;
        pool[0].aux = nullptr;
        pool[0].chain = 0;
        pool[0].flags = 0;
        return;
    }

    for (int i = 0; i < kRetailSyncTaskSlots; ++i) {
        if (pool[i].owner == task_desc) {
            pool[i].owner = nullptr;
            pool[i].aux = nullptr;
            pool[i].chain = 0;
            pool[i].flags = 0;
            return;
        }
    }
}

void retail_synctask_run_slots(RetailSyncTaskSlot* pool, void* tick_ctx) {
    (void)tick_ctx;
    if (!pool) {
        return;
    }

    for (int i = 0; i < kRetailSyncTaskSlots; ++i) {
        RetailSyncTaskSlot* slot = &pool[i];
        if (!slot->owner) {
            continue;
        }
        auto* desc = (RetailSyncTaskDesc*)slot->owner;
        if (desc->fn) {
            desc->fn(desc->ctx);
        }
    }
}

} // extern "C"
