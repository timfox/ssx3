#include "common.h"
#include <cstring>

#ifdef SSX3_HOST
extern "C" {
#endif

void* operator_new1(unsigned long size);

#ifdef SSX3_HOST
static unsigned char g_spring_node_template[0x50];
unsigned char* D_004FF120 = g_spring_node_template;
#else
extern unsigned char D_004FF120[0x50];
#endif

// Retail @ 0x00353F00 — allocate count * 0x50 nodes and clone template D_004FF120.
INCLUDE_ASM("mem/rom21_cSpring", cSpring_setupNodes);
#ifdef SKIP_ASM
void cSpring_setupNodes(void* spring) {
    int count = *(int*)((char*)spring + 0);
    if (count <= 0) {
        return;
    }
    void* nodes = operator_new1((unsigned long)(count * 0x50));
    if (nodes == 0) {
        return;
    }
    *(void**)((char*)spring + 0x24) = nodes;
    char* tmpl = (char*)D_004FF120;
    char* base = (char*)nodes;
    for (int i = 0; i < count; i++) {
        char* node = base + (i * 0x50);
        memcpy(node, tmpl, 0x50);
        *(int*)(node + 0x20) = *(int*)(tmpl + 0x20);
        *(int*)(node + 0x24) = i;
        *(int*)(node + 0x40) = *(int*)(tmpl + 0x40);
    }
}
#endif

#ifdef SSX3_HOST
}
#endif
