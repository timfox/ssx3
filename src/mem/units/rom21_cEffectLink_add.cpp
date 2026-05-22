#include "common.h"
// @objdiff-matched
#include "ps2_match.h"

#ifdef SKIP_ASM
extern "C" {


void cEffectLink_add(void** list_head, void* node) {
    void* prev = *list_head;
    if (prev != 0) {
        *(void**)((char*)prev + 4) = node;
    }
    *(void**)((char*)node + 4) = list_head;
    void* prev_head = *list_head;
    *(void**)((char*)node + 0) = prev_head;
    *list_head = node;
}

}
#endif
