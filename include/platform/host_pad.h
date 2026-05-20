#ifndef PLATFORM_HOST_PAD_H
#define PLATFORM_HOST_PAD_H

#include <cstdint>

namespace host {

/* Matches input.map Button0..Button15 ordering on disc. */
enum class PadButton : int {
    Select = 0,
    Start = 1,
    L3 = 2,
    R3 = 3,
    DPadRight = 4,
    DPadLeft = 5,
    DPadUp = 6,
    DPadDown = 7,
    Triangle = 8,
    Circle = 9,
    Cross = 10,
    Square = 11,
    L1 = 12,
    R1 = 13,
    L2 = 14,
    R2 = 15,
};

struct PadState {
    std::uint16_t buttons = 0;
    float left_x = 0.0f;
    float left_y = 0.0f;
    float right_x = 0.0f;
    float right_y = 0.0f;
};

void pad_init();
void pad_shutdown();
void pad_poll();

const PadState& pad_state(int port = 0);
bool pad_down(PadButton button, int port = 0);
bool pad_pressed(PadButton button, int port = 0);

} // namespace host

#ifdef __cplusplus
extern "C" {
#endif

/* Retail-facing pad read hook (expand when cInputPad is decompiled). */
int cInputPad_poll(int port);
unsigned short cInputPad_getButtons(int port);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_PAD_H */
