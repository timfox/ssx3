#include "common.h"
#include "ps2_match.h"

#ifdef SSX3_HOST
extern "C" {
#endif

// Retail @ 0x003454E8 — sum length fields on the active spline segment chain.
INCLUDE_ASM("mem/rom21_cSpline", cSpline_calcLength);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
float cSpline_calcLength(void* spline) {
    void* sub = *(void**)((char*)spline + 8);
    void* list = *(void**)((char*)sub + 0x68);
    int count = *(int*)((char*)list + 0x20);
    void* node = *(void**)((char*)list + 0x24);
    int idx = 1;
    if (idx < count) {
        int remaining = count - 1;
        while (remaining > 0) {
            remaining--;
            node = *(void**)((char*)node + 0x64);
        }
    }
    float a = *(float*)((char*)node + 0x84);
    float b = *(float*)((char*)node + 0xC);
    return a + b;
}
#endif

// Retail @ 0x00345570 — resolve replay frame offsets into spline runtime state.
INCLUDE_ASM("mem/rom21_cSpline", cSpline_readFromReplayFrame);
#ifdef SKIP_ASM
#ifndef SSX3_HOST
PS2_RETAIL_ASM_ONLY(
    cSpline_readFromReplayFrame,
    ".word 0x27bdffe0\n\t"
    ".word 0x00a0382d\n\t"
    ".word 0x7fb00010\n\t"
    ".word 0x24060008\n\t"
    ".word 0xffbf0000\n\t"
    ".word 0x0080802d\n\t"
    ".word 0x0200282d\n\t"
    ".word 0x8ce30000\n\t"
    ".word 0x84640010\n\t"
    ".word 0x8c620014\n\t"
    ".word 0x0040f809\n\t"
    ".word 0x00e42021\n\t"
    ".word 0x0c0b46f6\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8c430000\n\t"
    ".word 0x8e050000\n\t"
    ".word 0x8c640008\n\t"
    ".word 0x30a200ff\n\t"
    ".word 0x00021080\n\t"
    ".word 0x00441021\n\t"
    ".word 0x8c430000\n\t"
    ".word 0x10600008\n\t"
    ".word 0x00051202\n\t"
    ".word 0x8c640044\n\t"
    ".word 0x00021080\n\t"
    ".word 0x00441021\n\t"
    ".word 0x8c430000\n\t"
    ".word 0x00031202\n\t"
    ".word 0x14400002\n\t"
    ".word 0x00021080\n\t"
    ".word 0x0000102d\n\t"
    ".word 0x8c420024\n\t"
    ".word 0x8e040004\n\t"
    ".word 0x18800009\n\t"
    ".word 0xae020008\n\t"
    ".word 0x00000000\n\t"
    ".word 0x8e030008\n\t"
    ".word 0x2484ffff\n\t"
    ".word 0x8c620064\n\t"
    ".word 0x00000000\n\t"
    ".word 0x00000000\n\t"
    ".word 0x1480fffa\n\t"
    ".word 0xae020008\n\t"
    ".word 0x0c0d153a\n\t"
    ".word 0x0200202d\n\t"
    ".word 0xe600000c\n\t"
    ".word 0x7bb00010\n\t"
    ".word 0xdfbf0000\n\t"
    ".word 0x03e00008\n\t"
    ".word 0x27bd0020\n\t"
)
#else
extern void* func_002D1BD8(void);
extern float cSpline_calcLength(void* spline);

#ifdef SSX3_HOST
extern "C"
#endif
void cSpline_readFromReplayFrame(void* spline, void* frame_meta) {
    void* desc = *(void**)frame_meta;
    if (desc == 0) {
        return;
    }

    const unsigned short byte_off = *(unsigned short*)((char*)desc + 0x10);
    char* frame_bytes = (char*)frame_meta + byte_off;
    void* chain = *(void**)((char*)desc + 0x14);
    (void)func_002D1BD8();

    void* cursor = chain;
    for (int step_guard = 0; step_guard < 8 && cursor != 0; step_guard++) {
        if ((*(unsigned char*)cursor & 0xFFu) == 0) {
            const unsigned int skip = *(unsigned int*)((char*)cursor + 0x44);
            cursor = (char*)cursor + skip;
            void* link = *(void**)cursor;
            if (link == 0) {
                break;
            }
            const unsigned int link_skip = *(unsigned int*)((char*)link + 8);
            cursor = (char*)link + link_skip;
            if (*(void**)cursor == 0) {
                break;
            }
            cursor = *(void**)((char*)cursor + 0x24);
        } else {
            cursor = *(void**)((char*)cursor + 0x24);
        }

        if (cursor == *(void**)(frame_bytes + 4)) {
            break;
        }
    }

    int remaining = *(int*)(frame_bytes + 4);
    while (remaining >= 0) {
        *(void**)(frame_bytes + 8) = *(void**)((char*)cursor + 0x64);
        cSpline_calcLength(spline);
        remaining -= 1;
    }
}
#endif
#endif


#ifdef SSX3_HOST
}
#endif
