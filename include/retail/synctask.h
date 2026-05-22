#ifndef RETAIL_SYNCTASK_H
#define RETAIL_SYNCTASK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Retail @ 0x2E7448 — SYNCTASK_init zeros 0x100 bytes at D_0051ED98 (16 x 16-byte slots). */
void retail_memset_u8(void* dst, unsigned char fill, unsigned int size);

enum {
    kRetailSyncTaskSlots = 16,
    kRetailSyncTaskSlotBytes = 16,
    kRetailSyncTaskPoolBytes = kRetailSyncTaskSlots * kRetailSyncTaskSlotBytes,
};

/* Pool slot (SYNCTASK_add / del / run). */
typedef struct RetailSyncTaskSlot {
    void* owner; /* +0: task_desc pointer (del match key) */
    void* aux;   /* +4: second arg / aux field from retail add */
    int chain;   /* +8: links into D_00450dd0 list head */
    int flags;   /* +12: cleared on add */
} RetailSyncTaskSlot;

/* Caller descriptor; retail add stores owner = desc and aux from second arg when used. */
typedef struct RetailSyncTaskDesc {
    void* field0;
    void* field4;
    void (*fn)(void*);
    void* ctx;
} RetailSyncTaskDesc;

/* Returns 1 on success, -1 if pool full or invalid (retail v0). */
int SYNCTASK_add(void* task_desc);

#ifdef __cplusplus
}
#endif

#endif /* RETAIL_SYNCTASK_H */
