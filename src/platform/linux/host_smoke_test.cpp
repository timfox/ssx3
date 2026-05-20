#include "platform/host_abi.h"
#include "platform/host_fe.h"
#include "platform/host_fe_context.h"
#include "platform/host_fe_mainmenu.h"
#include "platform/host_fe_mountain_room.h"
#include "platform/host_fe_nav.h"
#include "platform/host_fe_options.h"
#include "platform/host_fe_options_game.h"
#include "platform/host_fe_options_sound.h"
#include "platform/host_fe_title.h"
#include "platform/host_menu_assets.h"
#include "platform/host_graphics.h"
#include "platform/host_big.h"
#include "platform/host_bigfile.h"
#include "platform/host_disc.h"
#include "platform/host_input.h"
#include "platform/host_io.h"
#include "platform/host_pad.h"
#include "platform/host_ssxapp.h"
#include "visualfx/crowdrender2d.h"

#include <iostream>

int tHashName32_getHashValue(unsigned int* out, char* str);
int GetHashValue32(char* str);
void BXsrand(unsigned int seed);
void BXrand();

namespace host {

namespace {

bool expect_hash32(const char* label, const char* input, unsigned int expected) {
    unsigned int hash = 0;
    const int result = tHashName32_getHashValue(&hash, const_cast<char*>(input));
    const int wrapped = GetHashValue32(const_cast<char*>(input));
    const bool ok = (hash == expected) && (result == static_cast<int>(expected)) &&
                    (wrapped == static_cast<int>(expected));
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "hash32 " << label << ": 0x" << std::hex << hash
              << std::dec << '\n';
    return ok;
}

bool smoke_big_audio() {
    auto* archive = big_open_cached("data/audio/audio.big");
    if (!archive) {
        std::cout << "[fail]    BIG open data/audio/audio.big\n";
        return false;
    }
    const BigEntry* entry = big_locate(*archive, "data\\audio\\BC_AmbLoop1.bnk");
    if (!entry || entry->size == 0) {
        std::cout << "[fail]    BIG_locate BC_AmbLoop1.bnk\n";
        return false;
    }
    std::cout << "[ok]      BIG audio.big: " << archive->entries.size() << " files, BC_AmbLoop1 @ 0x"
              << std::hex << entry->offset << " size=" << std::dec << entry->size << '\n';
    return true;
}

bool smoke_big_read() {
    const auto bytes = read_big_entry(*big_open_cached("data/audio/audio.big"), "data\\audio\\BC_AmbLoop1.bnk");
    if (!bytes || bytes->size() < 16) {
        std::cout << "[fail]    BIG read BC_AmbLoop1.bnk bytes\n";
        return false;
    }
    std::cout << "[ok]      BIG read BC_AmbLoop1.bnk: " << bytes->size() << " bytes\n";
    return true;
}

bool smoke_cbigfile() {
    HostBigFile handle{};
    cBigFile_cBigFile(&handle);
    if (!cBigFile_open(&handle, "data/audio/audio.big")) {
        std::cout << "[fail]    cBigFile_open audio.big\n";
        return false;
    }
    char header[16]{};
    const unsigned long n =
        cBigFile_read(&handle, "data\\audio\\BC_AmbLoop1.bnk", header, sizeof(header));
    cBigFile_close(&handle);
    if (n < 4) {
        std::cout << "[fail]    cBigFile_read BC_AmbLoop1.bnk\n";
        return false;
    }
    std::cout << "[ok]      cBigFile_open/read/close (host HAL)\n";
    return true;
}

bool smoke_ssxapp() {
    void* app = host_ssxapp_create();
    if (!app) {
        std::cout << "[fail]    host_ssxapp_create\n";
        return false;
    }
    cSSXApp_cSSXApp(app);
    const int ok = cSSXApp_init(app);
    host_ssxapp_destroy(app);
    if (!ok) {
        std::cout << "[fail]    cSSXApp_init\n";
        return false;
    }
    std::cout << "[ok]      cSSXApp_init (host HAL, retail 0x226900)\n";
    return true;
}

bool smoke_crowdrender2d() {
    void* anim = cCrowdRender2D_constructCrowdAnim2D(nullptr);
    if (!anim) {
        std::cout << "[fail]    cCrowdRender2D_constructCrowdAnim2D\n";
        return false;
    }
    cMemMan_free(anim);
    std::cout << "[ok]      crowdrender2d constructCrowdAnim2D (decompiled + host mem)\n";
    return true;
}

bool smoke_ps2_path_io() {
    const auto rel = ps2_path_to_disc_relative("cdrom0:\\data\\audio\\audio.big;1");
    if (!rel) {
        std::cout << "[fail]    ps2_path_to_disc_relative audio.big\n";
        return false;
    }
    if (rel->find("audio.big") == std::string::npos) {
        std::cout << "[fail]    ps2_path_to_disc_relative got: " << *rel << '\n';
        return false;
    }
    const auto bytes = read_asset_bytes(*rel);
    if (!bytes || bytes->empty()) {
        std::cout << "[fail]    read_asset_bytes loose audio.big\n";
        return false;
    }
    std::cout << "[ok]      ps2 path + read_asset_bytes: " << bytes->size() << " bytes\n";
    return true;
}

bool smoke_input_map() {
    void* map = host_input_map_create();
    if (!map) {
        std::cout << "[fail]    host_input_map_create\n";
        return false;
    }
    const int loaded = cInputMap_loadMapFile(map, "data/config/input.map");
    const int compiled = loaded ? cInputMap_compileMap(map) : 0;
    host_input_map_destroy(map);
    if (!loaded || !compiled) {
        std::cout << "[fail]    cInputMap load/compile input.map\n";
        return false;
    }
    std::cout << "[ok]      cInputMap load/compile data/config/input.map\n";
    return true;
}

bool smoke_fe_title() {
    void* fe = host_fe_title_create("assets/host/menu");
    if (!fe) {
        std::cout << "[fail]    host_fe_title_create\n";
        return false;
    }
    cFEStateTitle_onCreateScreen(fe);
    fe_set_frame_context(640, 448, 1.0f / 60.0f);
    cFEStateTitle_onUpdate(fe);
    cFEStateTitle_onDestroyScreen(fe);

    void* mm = host_fe_mainmenu_create("assets/host/menu");
    if (!mm) {
        std::cout << "[fail]    host_fe_mainmenu_create\n";
        host_fe_title_destroy(fe);
        return false;
    }
    cFEStateMainMenu_onCreateScreen(mm);
    cFEStateMainMenu_onUpdate(mm);
    cFEStateMainMenu_onDestroyScreen(mm);
    host_fe_mainmenu_destroy(mm);
    host_fe_title_destroy(fe);

    if (!host_menu_assets_ready("assets/host/menu")) {
        std::cout << "[warn]    menu assets missing (fe_1.ssh auto-extract on --gfx)\n";
    }
    void* room = host_fe_mountain_room_create("assets/host/menu");
    if (room) {
        cFEStateMountainRoom_onCreateScreen(room);
        cFEStateMountainRoom_onUpdate(room);
        cFEStateMountainRoom_onDestroyScreen(room);
        host_fe_mountain_room_destroy(room);
    }
    void* opt = host_fe_options_create("assets/host/menu");
    if (opt) {
        cFEStateOptions_onCreateScreen(opt);
        cFEStateOptions_onUpdate(opt);
        cFEStateOptions_onDestroyScreen(opt);
        host_fe_options_destroy(opt);
    }

    void* og = host_fe_options_game_create("assets/host/menu");
    if (og) {
        cFEStateOptionsGame_onCreateScreen(og);
        cFEStateOptionsGame_onUpdate(og);
        cFEStateOptionsGame_onDestroyScreen(og);
        host_fe_options_game_destroy(og);
    }

    void* os = host_fe_options_sound_create("assets/host/menu");
    if (os) {
        cFEStateOptionsSound_onCreateScreen(os);
        cFEStateOptionsSound_onUpdate(os);
        cFEStateOptionsSound_onDestroyScreen(os);
        host_fe_options_sound_destroy(os);
    }

    std::cout << "[ok]      FE states: Title, MainMenu, MountainRoom, Options, Game, Sound\n";
    return true;
}

bool smoke_pad() {
    pad_init();
    pad_poll();
    const auto buttons = cInputPad_getButtons(0);
    pad_shutdown();
    std::cout << "[ok]      cInputPad_poll buttons=0x" << std::hex << buttons << std::dec << '\n';
    return true;
}

bool smoke_bxrandom() {
    BXsrand(1u);
    BXrand();
    BXrand();
    std::cout << "[ok]      bxrandom BXsrand/BXrand (decompiled + host BSS)\n";
    return true;
}

} // namespace

bool run_smoke_tests() {
    std::cout << "host smoke tests (decompiled C on Linux)\n";

    bool ok = true;
    ok = expect_hash32("empty", "", 0u) && ok;
    ok = expect_hash32("a", "a", 97u) && ok;

    const auto bootElf = disc().resolve_ps2_path("cdrom0:\\SLUS_207.72;1");
    if (bootElf && disc().exists("SLUS_207.72")) {
        std::cout << "[ok]      ps2 path resolve: " << *bootElf << '\n';
    } else {
        std::cout << "[fail]    ps2 path resolve for boot elf\n";
        ok = false;
    }

    const auto dataChildren = disc().list_children("data");
    if (dataChildren.empty()) {
        std::cout << "[warn]    disc/data has no entries (unexpected for a full rip)\n";
    } else {
        std::cout << "[ok]      disc/data entries: " << dataChildren.size() << '\n';
    }

    ok = smoke_big_audio() && ok;
    ok = smoke_big_read() && ok;
    ok = smoke_cbigfile() && ok;
    ok = smoke_ssxapp() && ok;
    ok = smoke_fe_title() && ok;
    ok = smoke_crowdrender2d() && ok;
    ok = smoke_ps2_path_io() && ok;
    ok = smoke_input_map() && ok;
    ok = smoke_pad() && ok;
    ok = smoke_bxrandom() && ok;

    return ok;
}

} // namespace host
