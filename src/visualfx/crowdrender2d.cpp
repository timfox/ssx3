#include "visualfx/crowdrender2d.h"

#ifdef SSX3_HOST

#include "platform/host_abi.h"
#include "crowdrender2d_data.cpp"

#include <stdint.h>

const char D_004875D8[] = "host";
extern int g_crowd_frame_tick;
extern int D_00445DD0;

struct CrowdAnim2D {
    int table_index;
    int field_04;
    int mode;
    int frame;
    int field_10;
    int interval;
    int last_tick;
    void* param;
};

static int crowd_host_frame_now(void) {
    return g_crowd_frame_tick;
}

void* cMemMan_alloc(unsigned long size, void* heap_tag, unsigned long flags, void* heap_state);

int cCrowdRender2D_cCrowdRender2D(int param_1) {
    cCrowdRender2D_init();
    return param_1;
}

void cCrowdRender2D__cCrowdRender2D(int* param_1, int param_2) {
    cCrowdRender2D_purge(param_1);
    if ((param_2 & 1) != 0) {
        operator_delete(param_1);
    }
}

int cCrowdRender2D_purge(int* param_1) {
    return *param_1;
}

void* cCrowdAnim2D_cCrowdAnim2D(void* memory, void* param_1) {
    CrowdAnim2D* anim = static_cast<CrowdAnim2D*>(memory);
    if (!anim) {
        return 0;
    }
    anim->table_index = 0;
    anim->field_04 = 0;
    anim->mode = 4;
    anim->frame = 0;
    anim->field_10 = D_00445DD0;
    anim->interval = 3;
    anim->last_tick = crowd_host_frame_now();
    anim->param = param_1;
    return memory;
}

void* cCrowdRender2D_constructCrowdAnim2D(void* param) {
    void* memory = cMemMan_alloc(0x20u, reinterpret_cast<void*>(const_cast<char*>(D_004875D8)),
                                 0x20000000u, nullptr);
    return cCrowdAnim2D_cCrowdAnim2D(memory, param);
}

int cCrowdAnim2D_advanceFrame(void* /*anim*/, int frame, int max_frame, int* mode) {
    const int mode_index = *mode;
    if (mode_index >= 5) {
        return frame;
    }
    switch (mode_index) {
    case 0:
        frame += 1;
        return frame < max_frame ? frame : max_frame - 1;
    case 1:
        frame -= 1;
        return frame > -1 ? frame : 0;
    case 2:
        if (max_frame == 1) {
            return frame;
        }
        frame += 1;
        if (frame < max_frame) {
            return frame;
        }
        *mode = 3;
        return max_frame - 2;
    case 3:
        if (max_frame == 1) {
            return frame;
        }
        frame -= 1;
        if (frame >= 0) {
            return frame;
        }
        *mode = 2;
        return 1;
    case 4:
        if (max_frame == 1) {
            return frame;
        }
        frame += 1;
        return frame < max_frame ? frame : 0;
    default:
        return frame;
    }
}

void cCrowdAnim2D_update(void* anim_ptr) {
    (void)anim_ptr;
}

#endif /* SSX3_HOST */
