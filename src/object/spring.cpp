#include "common.h"
#include "ps2_match.h"
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
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cSpring_setupNodes,
    ".word 0x27bdffc0\n\t"
    ".word 0x3c050049\n\t"
    ".word 0x7fb10020\n\t"
    ".word 0x3c062000\n\t"
    ".word 0x7fb20010\n\t"
    ".word 0x0080882d\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x24040050\n\t"
    ".word 0x7fb00030\n\t"
    ".word 0x24a5e9c8\n\t"
    ".word 0x8e300000\n\t"
    ".word 0x0000382d\n\t"
    ".word 0x02040018\n\t"
    ".word 0x00002012\n\t"
    ".word 0x0c0c5f8c\n\t"
    ".word 0x26320024\n\t"
    ".word 0x12000003\n\t"
    ".word 0x2603ffff\n\t"
    ".word 0x2404ffff\n\t"
    ".word 0x00000000\n\t"
    ".word 0xae420000\n\t"
    ".word 0x0000282d\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x18400012\n\t"
    ".word 0x8e240024\n\t"
    ".word 0x3c020050\n\t"
    ".word 0x2446f120\n\t"
    ".word 0x00000000\n\t"
    ".word 0x78c30000\n\t"
    ".word 0x24a50001\n\t"
    ".word 0x7c830000\n\t"
    ".word 0x78c20000\n\t"
    ".word 0x7c820010\n\t"
    ".word 0x78c30000\n\t"
    ".word 0xac800024\n\t"
    ".word 0x7c830030\n\t"
    ".word 0xac800020\n\t"
    ".word 0xac800040\n\t"
    ".word 0x8e220000\n\t"
    ".word 0x00a2102a\n\t"
    ".word 0x1440fff3\n\t"
    ".word 0x24840050\n\t"
    ".word 0x7bb00030\n\t"
    ".word 0x7bb10020\n\t"
    ".word 0x7bb20010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0040\n\t"
)
#else
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
#endif


#ifdef SSX3_HOST
}
#endif
