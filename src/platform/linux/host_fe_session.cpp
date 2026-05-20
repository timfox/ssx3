#include "platform/host_fe_session.h"

#include "platform/host_fe.h"
#include "platform/host_fe_context.h"
#include "platform/host_fe_flow.h"
#include "platform/host_fe_mainmenu.h"
#include "platform/host_fe_mountain_room.h"
#include "platform/host_fe_nav.h"
#include "platform/host_fe_options.h"
#include "platform/host_fe_options_game.h"
#include "platform/host_fe_title.h"
#include "platform/host_menu_assets.h"
#include "platform/host_pad.h"

#include <SDL.h>

#include <iostream>

namespace host {
namespace {

struct FESession {
    void* title = nullptr;
    void* main_menu = nullptr;
    void* mountain_room = nullptr;
    void* options = nullptr;
    void* options_game = nullptr;
    FEScreen screen = FEScreen::Title;
};

void destroy_main_menu(FESession& session) {
    if (!session.main_menu) {
        return;
    }
    cFEStateMainMenu_onDestroyScreen(session.main_menu);
    host_fe_mainmenu_destroy(session.main_menu);
    session.main_menu = nullptr;
}

void destroy_mountain_room(FESession& session) {
    if (!session.mountain_room) {
        return;
    }
    cFEStateMountainRoom_onDestroyScreen(session.mountain_room);
    host_fe_mountain_room_destroy(session.mountain_room);
    session.mountain_room = nullptr;
}

void destroy_options(FESession& session) {
    if (!session.options) {
        return;
    }
    cFEStateOptions_onDestroyScreen(session.options);
    host_fe_options_destroy(session.options);
    session.options = nullptr;
}

void destroy_options_game(FESession& session) {
    if (!session.options_game) {
        return;
    }
    cFEStateOptionsGame_onDestroyScreen(session.options_game);
    host_fe_options_game_destroy(session.options_game);
    session.options_game = nullptr;
}

bool enter_main_menu(FESession& session, const std::string& asset_dir) {
    destroy_main_menu(session);
    session.main_menu = host_fe_mainmenu_create(asset_dir.c_str());
    if (!session.main_menu) {
        return false;
    }
    cFEStateMainMenu_onCreateScreen(session.main_menu);
    session.screen = FEScreen::MainMenu;
    std::cout << "[fe]      → cFEStateMainMenu (0x194AA8)\n";
    return true;
}

bool enter_mountain_room(FESession& session, const std::string& asset_dir) {
    destroy_mountain_room(session);
    session.mountain_room = host_fe_mountain_room_create(asset_dir.c_str());
    if (!session.mountain_room) {
        return false;
    }
    cFEStateMountainRoom_onCreateScreen(session.mountain_room);
    session.screen = FEScreen::MountainRoom;
    std::cout << "[fe]      → cFEStateMountainRoom (0x1D2990) Freeride\n";
    return true;
}

bool enter_options(FESession& session, const std::string& asset_dir) {
    destroy_options_game(session);
    destroy_options(session);
    session.options = host_fe_options_create(asset_dir.c_str());
    if (!session.options) {
        return false;
    }
    cFEStateOptions_onCreateScreen(session.options);
    session.screen = FEScreen::Options;
    std::cout << "[fe]      → cFEStateOptions (0x188800)\n";
    return true;
}

bool enter_options_game(FESession& session, const std::string& asset_dir) {
    destroy_options_game(session);
    session.options_game = host_fe_options_game_create(asset_dir.c_str());
    if (!session.options_game) {
        return false;
    }
    cFEStateOptionsGame_onCreateScreen(session.options_game);
    session.screen = FEScreen::OptionsGame;
    std::cout << "[fe]      → cFEStateOptionsGame (0x188F28)\n";
    return true;
}

void return_to_options_hub(FESession& session) {
    destroy_options_game(session);
    if (session.options) {
        cFEStateOptions_onCreateScreen(session.options);
        session.screen = FEScreen::Options;
        std::cout << "[fe]      ← cFEStateOptions\n";
    }
}

void return_to_title(FESession& session) {
    destroy_main_menu(session);
    destroy_mountain_room(session);
    destroy_options_game(session);
    destroy_options(session);
    if (session.title) {
        cFEStateTitle_onCreateScreen(session.title);
    }
    session.screen = FEScreen::Title;
    std::cout << "[fe]      ← cFEStateTitle\n";
}

void handle_title_input(FESession& session, const std::string& asset_dir) {
    if (pad_pressed(PadButton::Start) || pad_pressed(PadButton::Cross)) {
        if (session.title) {
            cFEStateTitle_onDestroyScreen(session.title);
        }
        if (!enter_main_menu(session, asset_dir)) {
            return_to_title(session);
        }
    }
}

void handle_main_menu_input(FESession& session, const std::string& asset_dir) {
    if (pad_pressed(PadButton::DPadUp)) {
        fe_mainmenu_navigate(session.main_menu, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_mainmenu_navigate(session.main_menu, 1);
    }
    if (pad_pressed(PadButton::Circle)) {
        return_to_title(session);
        return;
    }
    if (!pad_pressed(PadButton::Cross)) {
        return;
    }

    const int selected = fe_mainmenu_get_selected(session.main_menu);
    const char* label = fe_mainmenu_item_label(selected);
    switch (fe_mainmenu_action_for_index(selected)) {
    case FEMainMenuAction::MountainRoom:
        destroy_main_menu(session);
        if (!enter_mountain_room(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::Options:
        destroy_main_menu(session);
        if (!enter_options(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::Stub:
    default:
        std::cout << "[fe]      \"" << label << "\" not ported yet\n";
        break;
    }
}

void handle_mountain_room_input(FESession& session, const std::string& asset_dir) {
    if (pad_pressed(PadButton::DPadLeft)) {
        fe_mountain_room_navigate_peak(session.mountain_room, -1);
    }
    if (pad_pressed(PadButton::DPadRight)) {
        fe_mountain_room_navigate_peak(session.mountain_room, 1);
    }
    if (pad_pressed(PadButton::Circle)) {
        destroy_mountain_room(session);
        enter_main_menu(session, asset_dir);
        return;
    }
    if (pad_pressed(PadButton::Cross)) {
        std::cout << "[fe]      peak " << fe_mountain_room_get_peak(session.mountain_room)
                  << " — retail would open peak/event flow\n";
    }
}

void handle_options_input(FESession& session, const std::string& asset_dir) {
    if (pad_pressed(PadButton::DPadUp)) {
        fe_options_navigate(session.options, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_options_navigate(session.options, 1);
    }
    if (pad_pressed(PadButton::Circle) ||
        (pad_pressed(PadButton::Cross) && fe_options_selection_is_back(session.options))) {
        destroy_options_game(session);
        destroy_options(session);
        enter_main_menu(session, asset_dir);
        return;
    }
    if (!pad_pressed(PadButton::Cross)) {
        return;
    }

    const int selected = fe_options_get_selected(session.options);
    switch (fe_options_action_for_index(selected)) {
    case FEOptionsAction::OptionsGame:
        enter_options_game(session, asset_dir);
        break;
    case FEOptionsAction::Back:
        destroy_options(session);
        enter_main_menu(session, asset_dir);
        break;
    case FEOptionsAction::Stub:
    default:
        std::cout << "[fe]      \"" << fe_options_item_label(selected)
                  << "\" not ported (retail options sub-state)\n";
        break;
    }
}

void handle_options_game_input(FESession& session, const std::string& asset_dir) {
    (void)asset_dir;
    if (pad_pressed(PadButton::DPadUp)) {
        fe_options_game_navigate(session.options_game, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_options_game_navigate(session.options_game, 1);
    }
    if (pad_pressed(PadButton::Circle) ||
        (pad_pressed(PadButton::Cross) && fe_options_game_selection_is_back(session.options_game))) {
        return_to_options_hub(session);
        return;
    }
    if (pad_pressed(PadButton::Cross)) {
        std::cout << "[fe]      game option row " << fe_options_game_get_selected(session.options_game)
                  << " (widget event stub @ 0x189980)\n";
    }
}

void update_screen(FESession& session) {
    switch (session.screen) {
    case FEScreen::Title:
        cFEStateTitle_onUpdate(session.title);
        break;
    case FEScreen::MainMenu:
        cFEStateMainMenu_onUpdate(session.main_menu);
        break;
    case FEScreen::MountainRoom:
        cFEStateMountainRoom_onUpdate(session.mountain_room);
        break;
    case FEScreen::Options:
        cFEStateOptions_onUpdate(session.options);
        break;
    case FEScreen::OptionsGame:
        cFEStateOptionsGame_onUpdate(session.options_game);
        break;
    }
}

void handle_input(FESession& session, const std::string& asset_dir) {
    switch (session.screen) {
    case FEScreen::Title:
        handle_title_input(session, asset_dir);
        break;
    case FEScreen::MainMenu:
        handle_main_menu_input(session, asset_dir);
        break;
    case FEScreen::MountainRoom:
        handle_mountain_room_input(session, asset_dir);
        break;
    case FEScreen::Options:
        handle_options_input(session, asset_dir);
        break;
    case FEScreen::OptionsGame:
        handle_options_game_input(session, asset_dir);
        break;
    }
}

} // namespace

bool run_fe_menu_session(Renderer* renderer, const std::string& asset_dir) {
    if (!renderer) {
        return false;
    }
    if (!host_menu_ensure_assets(asset_dir)) {
        std::cerr << "[fe]      title assets unavailable (see host_menu_ensure_assets)\n";
        return false;
    }
    if (!renderer->init_main_menu(asset_dir)) {
        return false;
    }

    FESession session{};
    session.title = host_fe_title_create(asset_dir.c_str());
    if (!session.title) {
        renderer->shutdown_main_menu();
        return false;
    }

    cFEStateTitle_onCreateScreen(session.title);
    pad_init();

    std::cout << "[fe]      SSX3 front-end (retail FE states → Vulkan)\n";
    std::cout << "[fe]      Arrows/D-Pad · E or S confirm · Esc or D back · Space/Enter start\n";

    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last_ticks = SDL_GetPerformanceCounter();

    while (!renderer->should_close()) {
        const Uint64 now = SDL_GetPerformanceCounter();
        const float dt = static_cast<float>(now - last_ticks) / static_cast<float>(freq);
        last_ticks = now;

        renderer->poll_events();
        pad_poll();
        fe_set_frame_context(renderer->width(), renderer->height(), dt);
        handle_input(session, asset_dir);
        update_screen(session);

        if (!renderer->begin_frame()) {
            break;
        }
        renderer->end_frame();
        renderer->present();
    }

    return_to_title(session);
    destroy_main_menu(session);
    destroy_mountain_room(session);
    destroy_options_game(session);
    destroy_options(session);
    if (session.title) {
        cFEStateTitle_onDestroyScreen(session.title);
        host_fe_title_destroy(session.title);
    }
    renderer->shutdown_main_menu();
    pad_shutdown();
    return true;
}

} // namespace host
