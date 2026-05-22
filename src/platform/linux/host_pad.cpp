#include "platform/host_pad.h"

#include "platform/host_log.h"

#include <SDL.h>

#include <array>
#include <cstring>

namespace host {
namespace {

constexpr int kMaxPorts = 2;

std::array<PadState, kMaxPorts> g_current{};
std::array<PadState, kMaxPorts> g_previous{};
SDL_GameController* g_controller = nullptr;
bool g_ready = false;

bool keyboard_button(SDL_Scancode key) {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    return keys && keys[key];
}

void apply_keyboard(PadState& pad) {
    auto press = [&](PadButton button) {
        pad.buttons |= static_cast<std::uint16_t>(1u << static_cast<int>(button));
    };

    if (keyboard_button(SDL_SCANCODE_BACKSPACE)) {
        press(PadButton::Select);
    }
    if (keyboard_button(SDL_SCANCODE_RETURN) || keyboard_button(SDL_SCANCODE_SPACE)) {
        press(PadButton::Start);
    }
    if (keyboard_button(SDL_SCANCODE_E)) {
        press(PadButton::Cross);
    }
    if (keyboard_button(SDL_SCANCODE_ESCAPE)) {
        press(PadButton::Triangle);
    }
    if (keyboard_button(SDL_SCANCODE_UP)) {
        press(PadButton::DPadUp);
    }
    if (keyboard_button(SDL_SCANCODE_DOWN)) {
        press(PadButton::DPadDown);
    }
    if (keyboard_button(SDL_SCANCODE_LEFT)) {
        press(PadButton::DPadLeft);
    }
    if (keyboard_button(SDL_SCANCODE_RIGHT)) {
        press(PadButton::DPadRight);
    }
    if (keyboard_button(SDL_SCANCODE_S)) {
        press(PadButton::Cross);
    }
    if (keyboard_button(SDL_SCANCODE_A)) {
        press(PadButton::Square);
    }
    if (keyboard_button(SDL_SCANCODE_D)) {
        press(PadButton::Circle);
    }
    if (keyboard_button(SDL_SCANCODE_W)) {
        press(PadButton::Triangle);
    }
    /* Retail FE: Select = MenuKeyCancel */
    if (keyboard_button(SDL_SCANCODE_TAB)) {
        press(PadButton::Select);
    }
    if (keyboard_button(SDL_SCANCODE_Q)) {
        press(PadButton::L1);
    }
    if (keyboard_button(SDL_SCANCODE_E)) {
        press(PadButton::R1);
    }
}

void ensure_controller_open() {
    if (g_controller) {
        return;
    }
    if (SDL_NumJoysticks() <= 0) {
        return;
    }
    if (SDL_IsGameController(0)) {
        g_controller = SDL_GameControllerOpen(0);
    }
}

void apply_gamepad(PadState& pad) {
    ensure_controller_open();
    if (!g_controller) {
        return;
    }

    auto press = [&](SDL_GameControllerButton button, PadButton mapped) {
        if (SDL_GameControllerGetButton(g_controller, button)) {
            pad.buttons |= static_cast<std::uint16_t>(1u << static_cast<int>(mapped));
        }
    };

    press(SDL_CONTROLLER_BUTTON_BACK, PadButton::Select);
    press(SDL_CONTROLLER_BUTTON_START, PadButton::Start);
    press(SDL_CONTROLLER_BUTTON_DPAD_UP, PadButton::DPadUp);
    press(SDL_CONTROLLER_BUTTON_DPAD_DOWN, PadButton::DPadDown);
    press(SDL_CONTROLLER_BUTTON_DPAD_LEFT, PadButton::DPadLeft);
    press(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, PadButton::DPadRight);
    press(SDL_CONTROLLER_BUTTON_A, PadButton::Cross);
    press(SDL_CONTROLLER_BUTTON_B, PadButton::Circle);
    press(SDL_CONTROLLER_BUTTON_X, PadButton::Square);
    press(SDL_CONTROLLER_BUTTON_Y, PadButton::Triangle);
    press(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, PadButton::L1);
    press(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, PadButton::R1);

    const Sint16 lx = SDL_GameControllerGetAxis(g_controller, SDL_CONTROLLER_AXIS_LEFTX);
    const Sint16 ly = SDL_GameControllerGetAxis(g_controller, SDL_CONTROLLER_AXIS_LEFTY);
    const Sint16 rx = SDL_GameControllerGetAxis(g_controller, SDL_CONTROLLER_AXIS_RIGHTX);
    const Sint16 ry = SDL_GameControllerGetAxis(g_controller, SDL_CONTROLLER_AXIS_RIGHTY);
    constexpr float kAxisMax = 32767.0f;
    pad.left_x = static_cast<float>(lx) / kAxisMax;
    pad.left_y = static_cast<float>(ly) / kAxisMax;
    pad.right_x = static_cast<float>(rx) / kAxisMax;
    pad.right_y = static_cast<float>(ry) / kAxisMax;
}

} // namespace

void pad_init() {
    if (g_ready) {
        return;
    }
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
        host_log("pad", "SDL gamecontroller init failed (keyboard only)");
    }
    g_ready = true;
    host_log("pad", "host pad HAL ready (keyboard + gamepad)");
}

void pad_shutdown() {
    if (!g_ready) {
        return;
    }
    if (g_controller) {
        SDL_GameControllerClose(g_controller);
        g_controller = nullptr;
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
    g_ready = false;
}

void pad_poll() {
    if (!g_ready) {
        pad_init();
    }
    g_previous = g_current;
    for (PadState& state : g_current) {
        state = PadState{};
    }

    apply_keyboard(g_current[0]);
    apply_gamepad(g_current[0]);
}

const PadState& pad_state(int port) {
    if (port < 0 || port >= kMaxPorts) {
        port = 0;
    }
    return g_current[static_cast<size_t>(port)];
}

bool pad_down(PadButton button, int port) {
    const auto& state = pad_state(port);
    return (state.buttons & static_cast<std::uint16_t>(1u << static_cast<int>(button))) != 0;
}

bool pad_pressed(PadButton button, int port) {
    const bool now = pad_down(button, port);
    const auto& prev = g_previous[static_cast<size_t>(port < 0 || port >= kMaxPorts ? 0 : port)];
    const bool before =
        (prev.buttons & static_cast<std::uint16_t>(1u << static_cast<int>(button))) != 0;
    return now && !before;
}

} // namespace host
