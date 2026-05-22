#include "platform/host_fe_strings.h"

#include "platform/host_cfelocale.h"
#include "platform/host_fe_locale.h"

#include <cstdio>
#include <cstring>

namespace host {
namespace {

extern "C" int tHashName32_getHashValue(unsigned int* out, char* str);

const char* L(const char* key, const char* fallback) {
    if (const char* hit = fe_locale_get(key)) {
        return hit;
    }
    if (fallback && fallback[0]) {
        unsigned int hash = 0;
        tHashName32_getHashValue(&hash, const_cast<char*>(fallback));
        if (const char* by_hash = cFELocale_getString(nullptr, hash)) {
            return by_hash;
        }
        if (const char* retail = fe_locale_by_text(fallback)) {
            return retail;
        }
    }
    return fallback;
}

const char* retail_press_start_text() {
    return "press <start> to play";
}

} // namespace

const char* fe_string_title_press() {
    const char* raw = L("fe.title.press_start", retail_press_start_text());
    if (std::strstr(raw, "<start>") != nullptr) {
        return raw;
    }
    return retail_press_start_text();
}

const char* fe_string_title_copyright() {
    return L("fe.title.copyright", "Copyright Electronic Arts");
}

const char* fe_string_main_menu_title() {
    return L("fe.main_menu.title", "Main Menu");
}

int fe_mainmenu_item_count() {
    return 5;
}

const char* fe_mainmenu_label(int index) {
    static const char* kKeys[] = {
        "fe.main_menu.single_event",
        "fe.main_menu.conquer_mountain",
        "fe.main_menu.multi_play",
        "fe.main_menu.previews",
        "fe.main_menu.online",
    };
    static const char* kFallback[] = {
        "Single Event",
        "Conquer The Mountain",
        "Multi Play",
        "Previews",
        "Online",
    };
    if (index < 0 || index >= fe_mainmenu_item_count()) {
        return "?";
    }
    return L(kKeys[index], kFallback[index]);
}

const char* fe_string_hint_select() {
    return L("fe.hints.select", "Select");
}

const char* fe_string_hint_previous() {
    return L("fe.hints.previous", "Previous");
}

const char* fe_string_hint_options() {
    return L("fe.hints.options", "Options");
}

const char* fe_string_main_menu_hints() {
    static char buffer[192];
    std::snprintf(buffer,
                  sizeof(buffer),
                  "%s     %s     %s",
                  fe_string_hint_select(),
                  fe_string_hint_previous(),
                  fe_string_hint_options());
    return buffer;
}

const char* fe_mainmenu_description(int index) {
    static const char* kKeys[] = {
        "fe.main_menu.single_event.desc",
        "fe.main_menu.conquer_mountain.desc",
        "fe.main_menu.multi_play.desc",
        "fe.main_menu.previews.desc",
        "fe.main_menu.online.desc",
    };
    static const char* kFallback[] = {
        "Play any unlocked Single Event.",
        "Continue to peak selection.",
        "2 player head to head competition.",
        "View art, posters, cards, and videos earned.",
        "Compete against SSX 3 players around the world.",
    };
    if (index < 0 || index >= fe_mainmenu_item_count()) {
        return "";
    }
    return L(kKeys[index], kFallback[index]);
}

int fe_options_item_count() {
    return 5;
}

const char* fe_options_label(int index) {
    static const char* kKeys[] = {
        "fe.options.game",
        "fe.options.sound",
        "fe.options.controller",
        "fe.options.save_load",
        "fe.options.back",
    };
    static const char* kFallback[] = {
        "Game Options",
        "Sound Options",
        "Controller Settings",
        "Save/Load",
        "Back",
    };
    if (index < 0 || index >= fe_options_item_count()) {
        return "?";
    }
    return L(kKeys[index], kFallback[index]);
}

int fe_mountain_peak_count() {
    return 3;
}

const char* fe_string_mountain_select_peak() {
    return L("fe.mountain.select_peak", "Select Peak");
}

const char* fe_previews_screen_title() {
    return L("fe.previews.title", "Previews");
}

int fe_previews_item_count() {
    return 4;
}

const char* fe_previews_description(int index) {
    static const char* kFallback[] = {
        "View poster art you have unlocked.",
        "Browse trading cards you have collected.",
        "Watch videos you have earned.",
        "View cheat characters you have unlocked.",
    };
    if (index < 0 || index >= fe_previews_item_count()) {
        return "";
    }
    return kFallback[index];
}

const char* fe_previews_label(int index) {
    static const char* kKeys[] = {
        "fe.previews.posters",
        "fe.previews.trading_cards",
        "fe.previews.videos",
        "fe.previews.cheat_characters",
    };
    static const char* kFallback[] = {
        "Posters",
        "Trading Cards",
        "Videos",
        "Cheat Characters",
    };
    if (index < 0 || index >= fe_previews_item_count()) {
        return "?";
    }
    return L(kKeys[index], kFallback[index]);
}

const char* fe_multiplay_screen_title() {
    return L("fe.multiplay.title", "Select Multi Play Mode");
}

int fe_multiplay_item_count() {
    return 2;
}

const char* fe_multiplay_description(int index) {
    static const char* kFallback[] = {
        "Head-to-head on one console (retail 2-player flow).",
        "Multiplayer mode selection (retail lobby stub).",
    };
    if (index < 0 || index >= fe_multiplay_item_count()) {
        return "";
    }
    return kFallback[index];
}

const char* fe_multiplay_label(int index) {
    static const char* kKeys[] = {
        "fe.multiplay.two_player",
        "fe.multiplay.multiplayer",
    };
    static const char* kFallback[] = {
        "2 Player",
        "Multiplayer",
    };
    if (index < 0 || index >= fe_multiplay_item_count()) {
        return "?";
    }
    return L(kKeys[index], kFallback[index]);
}

const char* fe_single_event_screen_title() {
    return L("fe.single_event.title", "Single Event");
}

int fe_single_event_item_count() {
    return 4;
}

const char* fe_single_event_description(int index) {
    static const char* kFallback[] = {
        "Big Air competition at the selected venue.",
        "Race to the finish on the selected course.",
        "Slopestyle run judging tricks and lines.",
        "Backcountry freestyle outside the resort.",
    };
    if (index < 0 || index >= fe_single_event_item_count()) {
        return "";
    }
    return kFallback[index];
}

const char* fe_single_event_label(int index) {
    static const char* kKeys[] = {
        "fe.single_event.big_air",
        "fe.single_event.race",
        "fe.single_event.slopestyle",
        "fe.single_event.backcountry",
    };
    static const char* kFallback[] = {
        "Big Air",
        "Race",
        "Slopestyle",
        "Backcountry",
    };
    if (index < 0 || index >= fe_single_event_item_count()) {
        return "?";
    }
    return L(kKeys[index], kFallback[index]);
}

int fe_peak_room_item_count() {
    return 4;
}

const char* fe_peak_room_label(int index) {
    return fe_single_event_label(index);
}

const char* fe_peak_room_description(int index) {
    return fe_single_event_description(index);
}

const char* fe_options_controller_body() {
    return L("fe.options.controller.body",
             "Controller remapping and vibration (retail cFEStateOptionsController @ 0x18C6A0) "
             "is not ported on the host build yet.");
}

const char* fe_options_saveload_body() {
    return L("fe.options.saveload.body",
             "Memory card / profile save and load (retail cFEStateOptionsSaveLoad @ 0x18D280) "
             "is not ported on the host build yet.");
}

const char* fe_online_screen_title() {
    return fe_mainmenu_label(4);
}

const char* fe_online_body_text() {
    return fe_mainmenu_description(4);
}

const char* fe_mountain_peak_label(int index) {
    static const char* kKeys[] = {
        "fe.mountain.peak1",
        "fe.mountain.peak2",
        "fe.mountain.peak3",
    };
    static const char* kFallback[] = {
        "Peak 1",
        "Peak 2",
        "Peak 3",
    };
    if (index < 0 || index >= fe_mountain_peak_count()) {
        return "?";
    }
    return L(kKeys[index], kFallback[index]);
}

} // namespace host
