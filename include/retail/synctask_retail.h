#ifndef RETAIL_SYNCTASK_RETAIL_H
#define RETAIL_SYNCTASK_RETAIL_H

#include "retail/synctask.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Retail SYNCTASK_add (0x3E57F8): 3-register ABI when aux != -1.
 * Returns 1 on success, -1 if no free slot.
 */
int retail_synctask_add(void* task_desc, void* aux, int chain_delta);

void retail_synctask_del(void* task_desc);

void retail_synctask_run(void* tick_ctx);

#ifdef __cplusplus
}
#endif

#endif /* RETAIL_SYNCTASK_RETAIL_H */
