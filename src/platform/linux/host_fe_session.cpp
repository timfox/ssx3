#include "platform/host_fe_session.h"

#include "platform/host_fe_assets.h"
#include "platform/host_fe_layout_runtime.h"
#include "platform/host_cfelocale.h"
#include "platform/host_fe_locale.h"

#include "platform/host_fe.h"
#include "platform/host_fe_context.h"
#include "platform/host_fe_flow.h"
#include "platform/host_fe_mainmenu.h"
#include "platform/host_fe_mountain_room.h"
#include "platform/host_fe_nav.h"
#include "platform/host_fe_options.h"
#include "platform/host_fe_options_game.h"
#include "platform/host_fe_options_sound.h"
#include "platform/host_fe_submenu.h"
#include "platform/host_fe_message.h"
#include "platform/host_fe_transition.h"
#include "platform/host_ui_engine.h"
#include "platform/host_fe_title.h"
#include "platform/host_fe_strings.h"
#include "platform/host_menu_assets.h"
#include "platform/host_fe_font.h"
#include "platform/host_menu_audio.h"
#include "platform/host_pad.h"

#include <SDL.h>

#include <cstdio>
#include <functional>
#include <iostream>

namespace host {
namespace {

struct FESession {
    void* title = nullptr;
    void* main_menu = nullptr;
    void* mountain_room = nullptr;
    void* options = nullptr;
    void* options_game = nullptr;
    void* options_sound = nullptr;
    void* previews = nullptr;
    void* multiplay = nullptr;
    void* single_event = nullptr;
    void* peak_room = nullptr;
    void* message = nullptr;
    FEScreen message_return = FEScreen::MainMenu;
    FEScreen screen = FEScreen::Title;
    int input_grace_frames = 0;
};

void fe_arm_input_grace(FESession& session, int frames = 4) {
    if (frames > session.input_grace_frames) {
        session.input_grace_frames = frames;
    }
}

bool fe_consume_input_grace(FESession& session) {
    if (session.input_grace_frames <= 0) {
        return false;
    }
    --session.input_grace_frames;
    return true;
}

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

void destroy_options_sound(FESession& session) {
    if (!session.options_sound) {
        return;
    }
    cFEStateOptionsSound_onDestroyScreen(session.options_sound);
    host_fe_options_sound_destroy(session.options_sound);
    session.options_sound = nullptr;
}

void destroy_options_subscreens(FESession& session) {
    destroy_options_game(session);
    destroy_options_sound(session);
}

void destroy_previews(FESession& session) {
    if (!session.previews) {
        return;
    }
    cFEStateBonusMaterial_onDestroyScreen(session.previews);
    host_fe_previews_destroy(session.previews);
    session.previews = nullptr;
}

void destroy_multiplay(FESession& session) {
    if (!session.multiplay) {
        return;
    }
    cFEStateSelectMultiplayerMode_onDestroyScreen(session.multiplay);
    host_fe_multiplay_destroy(session.multiplay);
    session.multiplay = nullptr;
}

void destroy_single_event(FESession& session) {
    if (!session.single_event) {
        return;
    }
    cFEStateEventSelect_onDestroyScreen(session.single_event);
    host_fe_single_event_destroy(session.single_event);
    session.single_event = nullptr;
}

void destroy_peak_room(FESession& session) {
    if (!session.peak_room) {
        return;
    }
    cFEStatePeakRoom_onDestroyScreen(session.peak_room);
    host_fe_peak_room_destroy(session.peak_room);
    session.peak_room = nullptr;
}

void destroy_message(FESession& session) {
    if (!session.message) {
        return;
    }
    cFEStateOnlineMainMenu_onDestroyScreen(session.message);
    host_fe_message_destroy(session.message);
    session.message = nullptr;
}

void destroy_fe_submenus(FESession& session) {
    destroy_previews(session);
    destroy_multiplay(session);
    destroy_single_event(session);
    destroy_peak_room(session);
}

bool enter_main_menu(FESession& session, const std::string& asset_dir) {
    destroy_main_menu(session);
    session.main_menu = host_fe_mainmenu_create(asset_dir.c_str());
    if (!session.main_menu) {
        return false;
    }
    cFEStateMainMenu_onCreateScreen(session.main_menu);
    session.screen = FEScreen::MainMenu;
    host_menu_audio_on_screen(session.screen);
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
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStateMountainRoom (0x1D2990) Freeride\n";
    return true;
}

bool enter_options(FESession& session, const std::string& asset_dir) {
    destroy_options_subscreens(session);
    destroy_options(session);
    session.options = host_fe_options_create(asset_dir.c_str());
    if (!session.options) {
        return false;
    }
    cFEStateOptions_onCreateScreen(session.options);
    session.screen = FEScreen::Options;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStateOptions (0x188800)\n";
    return true;
}

bool enter_options_sound(FESession& session, const std::string& asset_dir) {
    destroy_options_sound(session);
    session.options_sound = host_fe_options_sound_create(asset_dir.c_str());
    if (!session.options_sound) {
        return false;
    }
    cFEStateOptionsSound_onCreateScreen(session.options_sound);
    session.screen = FEScreen::OptionsSound;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStateOptionsSound (0x18A298)\n";
    return true;
}

bool enter_previews(FESession& session, const std::string& asset_dir) {
    destroy_previews(session);
    session.previews = host_fe_previews_create(asset_dir.c_str());
    if (!session.previews) {
        return false;
    }
    cFEStateBonusMaterial_onCreateScreen(session.previews);
    session.screen = FEScreen::Previews;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStateBonusMaterial (0x1954D8)\n";
    return true;
}

bool enter_multiplay(FESession& session, const std::string& asset_dir) {
    destroy_multiplay(session);
    session.multiplay = host_fe_multiplay_create(asset_dir.c_str());
    if (!session.multiplay) {
        return false;
    }
    cFEStateSelectMultiplayerMode_onCreateScreen(session.multiplay);
    session.screen = FEScreen::MultiPlay;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStateSelectMultiplayerMode (0x1A3090)\n";
    return true;
}

bool enter_single_event(FESession& session, const std::string& asset_dir) {
    destroy_single_event(session);
    session.single_event = host_fe_single_event_create(asset_dir.c_str());
    if (!session.single_event) {
        return false;
    }
    cFEStateEventSelect_onCreateScreen(session.single_event);
    session.screen = FEScreen::SingleEvent;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStateEventSelect (0x186658)\n";
    return true;
}

bool enter_peak_room(FESession& session, const std::string& asset_dir, int peak_index) {
    destroy_peak_room(session);
    session.peak_room = host_fe_peak_room_create(asset_dir.c_str(), peak_index);
    if (!session.peak_room) {
        return false;
    }
    cFEStatePeakRoom_onCreateScreen(session.peak_room);
    session.screen = FEScreen::PeakRoom;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStatePeakRoom (0x1D3890) peak " << peak_index << "\n";
    return true;
}

bool enter_message(FESession& session,
                   const std::string& asset_dir,
                   void* screen_ptr,
                   FEScreen return_screen,
                   const char* log_line) {
    destroy_message(session);
    if (!screen_ptr) {
        return false;
    }
    session.message = screen_ptr;
    session.message_return = return_screen;
    cFEStateOnlineMainMenu_onCreateScreen(session.message);
    session.screen = FEScreen::Message;
    host_menu_audio_on_screen(session.screen);
    fe_arm_input_grace(session);
    std::cout << "[fe]      → " << log_line << "\n";
    return true;
}

bool enter_online(FESession& session, const std::string& asset_dir) {
    return enter_message(session,
                         asset_dir,
                         host_fe_online_create(asset_dir.c_str()),
                         FEScreen::MainMenu,
                         "cFEStateOnlineMainMenu (stub)");
}

bool enter_options_controller(FESession& session, const std::string& asset_dir) {
    return enter_message(session,
                         asset_dir,
                         host_fe_options_controller_create(asset_dir.c_str()),
                         FEScreen::Options,
                         "cFEStateOptionsController (0x18C6A0 stub)");
}

bool enter_options_saveload(FESession& session, const std::string& asset_dir) {
    return enter_message(session,
                         asset_dir,
                         host_fe_options_saveload_create(asset_dir.c_str()),
                         FEScreen::Options,
                         "cFEStateOptionsSaveLoad (0x18D280 stub)");
}

bool enter_preview_detail(FESession& session, const std::string& asset_dir, int index) {
    return enter_message(session,
                         asset_dir,
                         host_fe_preview_detail_create(asset_dir.c_str(), index),
                         FEScreen::Previews,
                         "cFEStateRewardGalleryBase (preview stub)");
}

bool enter_peak_event_stub(FESession& session, const std::string& asset_dir, int peak, int mode) {
    return enter_message(session,
                         asset_dir,
                         host_fe_peak_event_stub_create(asset_dir.c_str(), peak, mode),
                         FEScreen::PeakRoom,
                         "peak event / load stub");
}

bool enter_options_game(FESession& session, const std::string& asset_dir) {
    destroy_options_game(session);
    session.options_game = host_fe_options_game_create(asset_dir.c_str());
    if (!session.options_game) {
        return false;
    }
    cFEStateOptionsGame_onCreateScreen(session.options_game);
    session.screen = FEScreen::OptionsGame;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      → cFEStateOptionsGame (0x188F28)\n";
    return true;
}

void return_to_options_hub(FESession& session) {
    destroy_options_subscreens(session);
    if (session.options) {
        cFEStateOptions_onCreateScreen(session.options);
        session.screen = FEScreen::Options;
        host_menu_audio_on_screen(session.screen);
        std::cout << "[fe]      ← cFEStateOptions\n";
    }
}

void return_from_message(FESession& session, const std::string& asset_dir) {
    const FEScreen back = session.message_return;
    destroy_message(session);
    fe_arm_input_grace(session);
    switch (back) {
    case FEScreen::Options:
        return_to_options_hub(session);
        break;
    case FEScreen::Previews:
        if (session.previews) {
            session.screen = FEScreen::Previews;
            host_menu_audio_on_screen(session.screen);
        } else {
            enter_previews(session, asset_dir);
        }
        break;
    case FEScreen::PeakRoom:
        if (session.peak_room) {
            session.screen = FEScreen::PeakRoom;
            host_menu_audio_on_screen(session.screen);
        } else {
            enter_mountain_room(session, asset_dir);
        }
        break;
    case FEScreen::SingleEvent:
        if (session.single_event) {
            session.screen = FEScreen::SingleEvent;
            host_menu_audio_on_screen(session.screen);
        } else {
            enter_single_event(session, asset_dir);
        }
        break;
    case FEScreen::MainMenu:
    default:
        if (session.main_menu) {
            cFEStateMainMenu_onCreateScreen(session.main_menu);
            session.screen = FEScreen::MainMenu;
            host_menu_audio_on_screen(session.screen);
        } else {
            enter_main_menu(session, asset_dir);
        }
        break;
    }
}

void return_to_title(FESession& session) {
    destroy_main_menu(session);
    destroy_mountain_room(session);
    destroy_fe_submenus(session);
    destroy_message(session);
    destroy_options_subscreens(session);
    destroy_options(session);
    if (session.title) {
        cFEStateTitle_onCreateScreen(session.title);
    }
    session.screen = FEScreen::Title;
    host_menu_audio_on_screen(session.screen);
    std::cout << "[fe]      ← cFEStateTitle\n";
}

void poll_nav_vertical_sfx() {
    if (pad_pressed(PadButton::DPadUp) || pad_pressed(PadButton::DPadDown)) {
        host_menu_audio_play_sfx(MenuSfx::Navigate);
    }
}

void poll_nav_horizontal_sfx() {
    if (pad_pressed(PadButton::DPadLeft) || pad_pressed(PadButton::DPadRight)) {
        host_menu_audio_play_sfx(MenuSfx::Navigate);
    }
}

void handle_title_input(FESession& session, const std::string& asset_dir) {
    if (pad_pressed(PadButton::Start)) {
        host_menu_audio_play_sfx(MenuSfx::Start);
    }
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
    poll_nav_vertical_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        fe_mainmenu_navigate(session.main_menu, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_mainmenu_navigate(session.main_menu, 1);
    }
    if (pad_pressed(PadButton::Square)) {
        host_menu_audio_play_sfx(MenuSfx::Confirm);
        destroy_main_menu(session);
        if (!enter_options(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        return;
    }
    if (pad_pressed(PadButton::Triangle)) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        return_to_title(session);
        return;
    }
    if (!pad_pressed(PadButton::Cross)) {
        return;
    }
    host_menu_audio_play_sfx(MenuSfx::Confirm);

    const int selected = fe_mainmenu_get_selected(session.main_menu);
    const char* label = fe_mainmenu_item_label(selected);
    destroy_main_menu(session);
    switch (fe_mainmenu_action_for_index(selected)) {
    case FEMainMenuAction::MountainRoom:
        if (!enter_mountain_room(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::SingleEvent:
        if (!enter_single_event(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::MultiPlay:
        if (!enter_multiplay(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::Previews:
        if (!enter_previews(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::Online:
        if (!enter_online(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::Options:
        if (!enter_options(session, asset_dir)) {
            enter_main_menu(session, asset_dir);
        }
        break;
    case FEMainMenuAction::Stub:
    default:
        std::cout << "[fe]      \"" << label << "\" not ported yet\n";
        enter_main_menu(session, asset_dir);
        break;
    }
}

void handle_list_submenu_input(void* screen,
                               int (*get_selected)(void*),
                               void (*navigate)(void*, int),
                               const char* (*label_at)(int),
                               const std::function<void()>& on_back,
                               const char* retail_stub) {
    poll_nav_vertical_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        navigate(screen, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        navigate(screen, 1);
    }
    if (pad_pressed(PadButton::Triangle)) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        on_back();
        return;
    }
    if (!pad_pressed(PadButton::Cross) || !screen) {
        return;
    }
    host_menu_audio_play_sfx(MenuSfx::Confirm);
    const int selected = get_selected(screen);
    const char* item = label_at ? label_at(selected) : "?";
    std::cout << "[fe]      \"" << item << "\" — " << retail_stub << "\n";
}

void handle_previews_input(FESession& session, const std::string& asset_dir) {
    poll_nav_vertical_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        fe_previews_navigate(session.previews, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_previews_navigate(session.previews, 1);
    }
    if (pad_pressed(PadButton::Triangle)) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        destroy_previews(session);
        enter_main_menu(session, asset_dir);
        return;
    }
    if (!pad_pressed(PadButton::Cross) || !session.previews) {
        return;
    }
    host_menu_audio_play_sfx(MenuSfx::Confirm);
    const int selected = fe_previews_get_selected(session.previews);
    if (!enter_preview_detail(session, asset_dir, selected)) {
        std::cout << "[fe]      preview detail open failed\n";
    }
}

void handle_multiplay_input(FESession& session, const std::string& asset_dir) {
    handle_list_submenu_input(
        session.multiplay,
        fe_multiplay_get_selected,
        fe_multiplay_navigate,
        fe_multiplay_label,
        [&]() {
            destroy_multiplay(session);
            enter_main_menu(session, asset_dir);
        },
        "retail would open multiplayer setup (not ported)");
}

void handle_single_event_input(FESession& session, const std::string& asset_dir) {
    if (fe_consume_input_grace(session)) {
        return;
    }
    poll_nav_vertical_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        fe_single_event_navigate(session.single_event, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_single_event_navigate(session.single_event, 1);
    }
    if (pad_pressed(PadButton::Triangle)) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        destroy_single_event(session);
        enter_main_menu(session, asset_dir);
        return;
    }
    if (!pad_pressed(PadButton::Cross) || !session.single_event) {
        return;
    }
    host_menu_audio_play_sfx(MenuSfx::Confirm);
    const int mode = fe_single_event_get_selected(session.single_event);
    char body[256];
    std::snprintf(body,
                  sizeof(body),
                  "%s — retail event select/load not ported (cFEStateEventSelect).",
                  fe_single_event_label(mode));
    destroy_message(session);
    enter_message(session,
                  asset_dir,
                  host_fe_message_create(asset_dir.c_str(), fe_single_event_screen_title(), body),
                  FEScreen::SingleEvent,
                  "single event mode stub");
}

void handle_peak_room_input(FESession& session, const std::string& asset_dir) {
    poll_nav_vertical_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        fe_peak_room_navigate(session.peak_room, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_peak_room_navigate(session.peak_room, 1);
    }
    if (pad_pressed(PadButton::Triangle)) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        destroy_peak_room(session);
        if (!session.mountain_room) {
            enter_mountain_room(session, asset_dir);
        } else {
            cFEStateMountainRoom_onCreateScreen(session.mountain_room);
            session.screen = FEScreen::MountainRoom;
            host_menu_audio_on_screen(session.screen);
        }
        return;
    }
    if (!pad_pressed(PadButton::Cross) || !session.peak_room) {
        return;
    }
    host_menu_audio_play_sfx(MenuSfx::Confirm);
    const int peak = fe_peak_room_get_peak(session.peak_room);
    const int mode = fe_peak_room_get_selected(session.peak_room);
    if (!enter_peak_event_stub(session, asset_dir, peak, mode)) {
        std::cout << "[fe]      peak event stub open failed\n";
    }
}

void handle_message_input(FESession& session, const std::string& asset_dir) {
    if (fe_consume_input_grace(session)) {
        return;
    }
    if (pad_pressed(PadButton::Triangle)) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        return_from_message(session, asset_dir);
    }
}

void handle_mountain_room_input(FESession& session, const std::string& asset_dir) {
    poll_nav_vertical_sfx();
    poll_nav_horizontal_sfx();
    if (pad_pressed(PadButton::DPadLeft)) {
        fe_mountain_room_navigate_peak(session.mountain_room, -1);
    }
    if (pad_pressed(PadButton::DPadRight)) {
        fe_mountain_room_navigate_peak(session.mountain_room, 1);
    }
    if (pad_pressed(PadButton::Triangle)) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        destroy_mountain_room(session);
        enter_main_menu(session, asset_dir);
        return;
    }
    if (pad_pressed(PadButton::Cross)) {
        host_menu_audio_play_sfx(MenuSfx::Confirm);
        const int peak = fe_mountain_room_get_peak(session.mountain_room);
        if (!enter_peak_room(session, asset_dir, peak)) {
            std::cout << "[fe]      peak room create failed\n";
        }
    }
}

void handle_options_input(FESession& session, const std::string& asset_dir) {
    poll_nav_vertical_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        fe_options_navigate(session.options, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_options_navigate(session.options, 1);
    }
    if (pad_pressed(PadButton::Triangle) ||
        (pad_pressed(PadButton::Cross) && fe_options_selection_is_back(session.options))) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        destroy_options_subscreens(session);
        destroy_options(session);
        enter_main_menu(session, asset_dir);
        return;
    }
    if (!pad_pressed(PadButton::Cross)) {
        return;
    }
    host_menu_audio_play_sfx(MenuSfx::Confirm);

    const int selected = fe_options_get_selected(session.options);
    switch (fe_options_action_for_index(selected)) {
    case FEOptionsAction::OptionsGame:
        enter_options_game(session, asset_dir);
        break;
    case FEOptionsAction::OptionsSound:
        enter_options_sound(session, asset_dir);
        break;
    case FEOptionsAction::OptionsController:
        enter_options_controller(session, asset_dir);
        break;
    case FEOptionsAction::OptionsSaveLoad:
        enter_options_saveload(session, asset_dir);
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
    poll_nav_vertical_sfx();
    poll_nav_horizontal_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        fe_options_game_navigate(session.options_game, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_options_game_navigate(session.options_game, 1);
    }
    if (pad_pressed(PadButton::DPadLeft)) {
        fe_options_game_adjust_value(session.options_game, -1);
    }
    if (pad_pressed(PadButton::DPadRight)) {
        fe_options_game_adjust_value(session.options_game, 1);
    }
    if (pad_pressed(PadButton::Triangle) ||
        (pad_pressed(PadButton::Cross) && fe_options_game_selection_is_back(session.options_game))) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        return_to_options_hub(session);
        return;
    }
    if (pad_pressed(PadButton::Cross)) {
        host_menu_audio_play_sfx(MenuSfx::Confirm);
        fe_options_game_adjust_value(session.options_game, 1);
    }
}

void handle_options_sound_input(FESession& session, const std::string& asset_dir) {
    (void)asset_dir;
    poll_nav_vertical_sfx();
    poll_nav_horizontal_sfx();
    if (pad_pressed(PadButton::DPadUp)) {
        fe_options_sound_navigate(session.options_sound, -1);
    }
    if (pad_pressed(PadButton::DPadDown)) {
        fe_options_sound_navigate(session.options_sound, 1);
    }
    if (pad_pressed(PadButton::DPadLeft)) {
        fe_options_sound_adjust_value(session.options_sound, -1);
    }
    if (pad_pressed(PadButton::DPadRight)) {
        fe_options_sound_adjust_value(session.options_sound, 1);
    }
    if (pad_pressed(PadButton::Triangle) ||
        (pad_pressed(PadButton::Cross) && fe_options_sound_selection_is_back(session.options_sound))) {
        host_menu_audio_play_sfx(MenuSfx::Back);
        return_to_options_hub(session);
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
    case FEScreen::OptionsSound:
        cFEStateOptionsSound_onUpdate(session.options_sound);
        break;
    case FEScreen::Previews:
        cFEStateBonusMaterial_onUpdate(session.previews);
        break;
    case FEScreen::MultiPlay:
        cFEStateSelectMultiplayerMode_onUpdate(session.multiplay);
        break;
    case FEScreen::SingleEvent:
        cFEStateEventSelect_onUpdate(session.single_event);
        break;
    case FEScreen::PeakRoom:
        cFEStatePeakRoom_onUpdate(session.peak_room);
        break;
    case FEScreen::Message:
        cFEStateOnlineMainMenu_onUpdate(session.message);
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
    case FEScreen::OptionsSound:
        handle_options_sound_input(session, asset_dir);
        break;
    case FEScreen::Previews:
        handle_previews_input(session, asset_dir);
        break;
    case FEScreen::MultiPlay:
        handle_multiplay_input(session, asset_dir);
        break;
    case FEScreen::SingleEvent:
        handle_single_event_input(session, asset_dir);
        break;
    case FEScreen::PeakRoom:
        handle_peak_room_input(session, asset_dir);
        break;
    case FEScreen::Message:
        handle_message_input(session, asset_dir);
        break;
    }
}

} // namespace

bool run_fe_menu_session(Renderer* renderer, const std::string& asset_dir) {
    if (!renderer) {
        return false;
    }
    fe_ensure_host_assets();
    if (!host_menu_ensure_assets(asset_dir)) {
        std::cerr << "[fe]      title assets unavailable (see host_menu_ensure_assets)\n";
        return false;
    }
    fe_locale_init();
    cFELocale_addFile(nullptr, "data/locale/cmnamer.loc");
    cFELocale_addFile(nullptr, "data/locale/feamer.loc");
    fe_layout_load();
    {
        void* ui = host_ui_engine_create();
        if (ui) {
            cUIEngine_loadFile(ui, "data/ui/fe.lui");
            host_ui_engine_destroy(ui);
        }
    }
    host_menu_audio_ensure();
    host_fe_font_ensure(asset_dir);
    if (!host_fe_font_load(asset_dir)) {
        std::cerr << "[font]    no bitmap font — menu text uses stb fallback\n";
        std::cerr << "[font]    retail: python3 scripts/extract_menu_font.py --disc disc\n";
        std::cerr << "[font]    custom: python3 scripts/bake_host_font.py\n";
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
    host_menu_audio_start();
    host_menu_audio_on_screen(FEScreen::Title);

    std::cout << "[fe]      SSX3 front-end (retail FE states → Vulkan + decomp VFX/audio)\n";
    std::cout << "[fe]      D-Pad navigate · Cross/E confirm · Triangle/W back · Square/A options · Start/Space title\n";

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
    destroy_fe_submenus(session);
    destroy_message(session);
    destroy_options_subscreens(session);
    destroy_options(session);
    if (session.title) {
        cFEStateTitle_onDestroyScreen(session.title);
        host_fe_title_destroy(session.title);
    }
    host_menu_audio_stop();
    renderer->shutdown_main_menu();
    pad_shutdown();
    return true;
}

} // namespace host
