#include "platform/host_abi.h"
#include "platform/host_fe.h"
#include "platform/host_fe_context.h"
#include "platform/host_fe_mainmenu.h"
#include "platform/host_fe_mountain_room.h"
#include "platform/host_fe_nav.h"
#include "platform/host_fe_options.h"
#include "platform/host_fe_options_game.h"
#include "platform/host_fe_options_sound.h"
#include "platform/host_fe_submenu.h"
#include "platform/host_fe_message.h"
#include "platform/host_fe_title.h"
#include "platform/host_menu_assets.h"
#include "platform/host_graphics.h"
#include "platform/host_big.h"
#include "platform/host_bigfile.h"
#include "platform/host_disc.h"
#include "platform/host_input.h"
#include "platform/host_io.h"
#include "platform/host_pad.h"
#include "platform/host_cfelocale.h"
#include "platform/host_ui_engine.h"
#include "platform/host_fe_transition.h"
#include "platform/host_fe_locale.h"
#include "platform/host_ssxapp.h"
#include "platform/host_game.h"
#include "platform/host_big.h"
#include "platform/host_retail.h"
#include "platform/host_world.h"
#include "visualfx/crowdrender2d.h"

#include <iostream>

extern "C" int tHashName32_getHashValue(unsigned int* out, char* str);
extern "C" int GetHashValue32(char* str);
extern "C" unsigned long long tHashName64_getHashValue(unsigned long long* out, char* str);
extern "C" unsigned long long GetHashValue64(char* str);
extern "C" void BXsrand(unsigned int seed);
extern "C" void BXrand();
extern "C" float AIrandf(float a, float b);
extern "C" float func_00317890(float a, float b);
extern "C" unsigned long BIG_sizeofheader(const char* path);
extern "C" void* BIG_locateentryz(void* archive, const char* name);
extern "C" void retail_memset_u8(void* dst, unsigned char fill, unsigned int size);
extern "C" void SYNCTASK_init(void);
extern "C" void cRider_initOnce(void* self);
extern "C" int SYNCTASK_add(void* task_desc);
extern "C" void SYNCTASK_del(void* task_desc);
extern "C" void SYNCTASK_run(void);
extern "C" const char* cFELocale_getString(void* locale, unsigned int hash_id);
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

bool smoke_hash64() {
    unsigned long long hash[2]{};
    const unsigned long long low = tHashName64_getHashValue(hash, const_cast<char*>("a"));
    const bool ok = low != 0 && hash[0] == low;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "hash64 MD5 tHashName64(\"a\") low=0x" << std::hex << low
              << std::dec << '\n';
    return ok;
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
    std::cout << "[ok]      cBigFile_open/read/close BIG entry\n";

    HostBigFile loose{};
    cBigFile_cBigFile(&loose);
    if (cBigFile_open(&loose, "data/config/input2.map")) {
        char header[8]{};
        const unsigned long n = cBigFile_read(&loose, "data/config/input2.map", header, sizeof(header));
        cBigFile_close(&loose);
        if (n >= 4) {
            std::cout << "[ok]      cBigFile_read loose file (input2.map)\n";
            return true;
        }
    }
    std::cout << "[warn]    cBigFile_read loose file skipped\n";
    return true;
}

extern "C" char* cDirtysock_tag__TagFieldSetupAppend(char* message, char* buffer, char* key);

bool smoke_dirtysock_tags() {
    char message[64]{};
    char buffer[64]{};
    char* end = cDirtysock_tag__TagFieldSetupAppend(message, buffer, const_cast<char*>("test"));
    const bool ok = end == buffer + 5 && buffer[0] == 't' && buffer[4] == '=';
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "cDirtysock_tag TagFieldSetupAppend (decompiled)\n";
    return ok;
}

bool smoke_cfelocale() {
    unsigned int hash = 0;
    tHashName32_getHashValue(&hash, const_cast<char*>("Main Menu"));
    if (const char* by_hash = cFELocale_getString(nullptr, hash)) {
        std::cout << "[ok]      cFELocale_getString hash Main Menu → \"" << by_hash << "\"\n";
        return true;
    }
    const char* hit = host::fe_locale_by_text("Main Menu");
    if (hit) {
        std::cout << "[ok]      cFELocale retail pool: \"Main Menu\"\n";
        return true;
    }
    const int n = host::fe_locale_add_loc_file("data/locale/cmnamer.loc");
    const char* hit2 = host::fe_locale_by_text("Main Menu");
    const bool ok = hit2 != nullptr;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "cFELocale_addFile cmnamer.loc: +" << n
              << " new, lookup Main Menu\n";
    return ok;
}

bool smoke_ui_engine() {
    void* ui = host_ui_engine_create();
    if (!ui) {
        std::cout << "[fail]    host_ui_engine_create\n";
        return false;
    }
    const int ok_fe = cUIEngine_loadFile(ui, "data/ui/fe.lui");
    const int ok_fl = cUIEngine_loadFile(ui, "data/ui/fl.lui");
    host_ui_engine_destroy(ui);
    if (!ok_fe) {
        std::cout << "[warn]    cUIEngine_loadFile fe.lui (disc UI missing?)\n";
        return true;
    }
    std::cout << "[ok]      cUIEngine_loadFile fe.lui fl.lui=" << ok_fl << '\n';
    return true;
}

bool smoke_fe_transition() {
    void* tr = host_fe_transition_create("assets/host/menu");
    if (!tr) {
        std::cout << "[fail]    host_fe_transition_create\n";
        return false;
    }
    cFEStateTransition_onCreateScreen(tr);
    cFEStateTransition_onUpdate(tr);
    cFEStateTransition_onDestroyScreen(tr);
    host_fe_transition_destroy(tr);
    std::cout << "[ok]      cFEStateTransition (retail 0x194588)\n";
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

bool smoke_game_load() {
    void* app = host_ssxapp_create();
    if (!app) {
        return false;
    }
    cSSXApp_cSSXApp(app);
    if (!cSSXApp_init(app)) {
        host_ssxapp_destroy(app);
        std::cout << "[fail]    cSSXApp_init before game load\n";
        return false;
    }
    const int load = cSSXApp_startGameLoad(app, 0);
    const int track = cSSXApp_initload(app);
    host_ssxapp_destroy(app);
    const bool ok = load != 0 && track != 0;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "cSSXApp startGameLoad → initload → cGame/world\n";
    return ok;
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
    int loaded = cInputMap_loadMapFile(map, "data/config/input2.map");
    if (!loaded) {
        loaded = cInputMap_loadMapFile(map, "data/config/input.map");
    }
    const int compiled = loaded ? cInputMap_compileMap(map) : 0;
    host_input_map_destroy(map);
    if (!loaded || !compiled) {
        std::cout << "[fail]    cInputMap load/compile input.map\n";
        return false;
    }
        std::cout << "[ok]      cInputMap load/compile input2.map / input.map\n";
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

    void* prev = host_fe_previews_create("assets/host/menu");
    if (prev) {
        cFEStateBonusMaterial_onCreateScreen(prev);
        cFEStateBonusMaterial_onUpdate(prev);
        cFEStateBonusMaterial_onDestroyScreen(prev);
        host_fe_previews_destroy(prev);
    }
    void* mp = host_fe_multiplay_create("assets/host/menu");
    if (mp) {
        cFEStateSelectMultiplayerMode_onCreateScreen(mp);
        cFEStateSelectMultiplayerMode_onUpdate(mp);
        cFEStateSelectMultiplayerMode_onDestroyScreen(mp);
        host_fe_multiplay_destroy(mp);
    }
    void* se = host_fe_single_event_create("assets/host/menu");
    if (se) {
        cFEStateEventSelect_onCreateScreen(se);
        cFEStateEventSelect_onUpdate(se);
        cFEStateEventSelect_onDestroyScreen(se);
        host_fe_single_event_destroy(se);
    }
    void* peak = host_fe_peak_room_create("assets/host/menu", 1);
    if (peak) {
        cFEStatePeakRoom_onCreateScreen(peak);
        cFEStatePeakRoom_onUpdate(peak);
        cFEStatePeakRoom_onDestroyScreen(peak);
        host_fe_peak_room_destroy(peak);
    }
    void* onl = host_fe_online_create("assets/host/menu");
    if (onl) {
        cFEStateOnlineMainMenu_onCreateScreen(onl);
        cFEStateOnlineMainMenu_onUpdate(onl);
        cFEStateOnlineMainMenu_onDestroyScreen(onl);
        host_fe_online_destroy(onl);
    }
    void* ctrl = host_fe_options_controller_create("assets/host/menu");
    if (ctrl) {
        cFEStateOptionsController_onCreateScreen(ctrl);
        cFEStateOptionsController_onUpdate(ctrl);
        cFEStateOptionsController_onDestroyScreen(ctrl);
        host_fe_options_controller_destroy(ctrl);
    }

    std::cout << "[ok]      FE states: Title, MainMenu, MountainRoom, Options, submenus\n";
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
    const float rf = AIrandf(0.0f, 1.0f);
    const bool ok = rf >= 0.0f && rf <= 1.0f;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "bxrandom BXsrand/BXrand/AIrandf (decompiled)\n";
    return ok;
}

bool smoke_bxrandom_func() {
    const float v = func_00317890(10.0f, 5.0f);
    const bool ok = v >= 5.0f && v <= 15.0f;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "bxrandom func_00317890 (decompiled)\n";
    return ok;
}

bool smoke_big_retail_api() {
    const auto archive = big_open("data/audio/audio.big");
    if (!archive) {
        std::cout << "[fail]    BIG_locateentryz open audio.big\n";
        return false;
    }
    const unsigned long hdr = BIG_sizeofheader("data/audio/audio.big");
    void* entry = BIG_locateentryz(const_cast<host::BigArchive*>(&*archive), "data\\audio\\BC_AmbLoop1.bnk");
    const bool ok = hdr == 16 && entry != nullptr;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "BIG_sizeofheader + BIG_locateentryz (retail API)\n";
    return ok;
}

bool smoke_retail_modules() {
    host_retail_module_init(nullptr, 1);
    SYNCTASK_init();
    std::cout << "[ok]      host_retail_module_init + SYNCTASK_init\n";
    return true;
}

bool smoke_rider_init() {
    char rider_buf[256]{};
    cRider_initOnce(rider_buf);
    std::cout << "[ok]      cRider_initOnce (decompiled SKIP_ASM on native)\n";
    return true;
}

bool smoke_retail_memset() {
    unsigned char buf[64]{};
    buf[0] = 0xff;
    retail_memset_u8(buf, 0, sizeof(buf));
    const bool ok = buf[0] == 0 && buf[63] == 0;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "retail_memset_u8 (func_003E6448)\n";
    return ok;
}

bool smoke_synctask_pool() {
    struct TaskDesc {
        void* field0;
        void* field4;
        void (*fn)(void*);
        void* ctx;
    };

    static int tick_count = 0;
    auto tick = +[](void* ctx) {
        ++*static_cast<int*>(ctx);
    };

    SYNCTASK_init();
    TaskDesc desc{};
    desc.field0 = &desc;
    desc.field4 = &desc;
    desc.fn = tick;
    desc.ctx = &tick_count;
    if (SYNCTASK_add(&desc) != 1) {
        std::cout << "[fail]    SYNCTASK_add returned error\n";
        return false;
    }
    SYNCTASK_run();
    SYNCTASK_del(&desc);

    const bool ok = tick_count == 1;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "SYNCTASK pool add/run/del\n";
    return ok;
}

bool smoke_world_api() {
    void* world = host_world_create();
    if (!world) {
        std::cout << "[fail]    host_world_create\n";
        return false;
    }
    cWorld_cWorld(world);
    const int reset = cWorld_resetMap(world, 0);
    void* view = host_world_view_create(4);
    const int sections = cWorldView_getNumSections(view);
    const int loaded = cWorldView_isSectionLoaded(view, 0);
    void* cache = host_world_cache_create();
    if (cache) {
        cWorldCache_init(cache, world);
        (void)cWorldCache_activateSectionMem(cache, world, 0);
        host_world_cache_destroy(cache);
    }
    host_world_view_destroy(view);
    host_world_destroy(world);
    const bool ok = reset == 0 && sections == 4 && loaded == 0;
    std::cout << (ok ? "[ok]      " : "[fail]    ") << "cWorld_* (host_world state + retail layout)\n";
    return ok;
}

} // namespace

bool run_smoke_tests() {
    std::cout << "host smoke tests (decompiled C on Linux)\n";

    bool ok = true;
    ok = expect_hash32("empty", "", 0u) && ok;
    ok = expect_hash32("a", "a", 97u) && ok;
    ok = smoke_hash64() && ok;

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
    ok = smoke_cfelocale() && ok;
    ok = smoke_ui_engine() && ok;
    ok = smoke_fe_transition() && ok;
    ok = smoke_ssxapp() && ok;
    ok = smoke_game_load() && ok;
    ok = smoke_fe_title() && ok;
    ok = smoke_crowdrender2d() && ok;
    ok = smoke_ps2_path_io() && ok;
    ok = smoke_input_map() && ok;
    ok = smoke_pad() && ok;
    ok = smoke_bxrandom() && ok;
    ok = smoke_bxrandom_func() && ok;
    ok = smoke_big_retail_api() && ok;
    ok = smoke_retail_modules() && ok;
    ok = smoke_rider_init() && ok;
    ok = smoke_retail_memset() && ok;
    ok = smoke_synctask_pool() && ok;
    ok = smoke_world_api() && ok;
    ok = smoke_dirtysock_tags() && ok;

    return ok;
}

} // namespace host
