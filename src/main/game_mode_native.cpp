// Retail cGameModeMan @ 0x237CF8+ / cGameViewMan @ 0x22E3B8+ — native until decompiled from ObjDiff units.

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_game_mode_state.h"
#include "platform/host_log.h"

#include <cstdio>

extern "C" {
#endif

namespace {

host::HostGameModeMan g_game_mode_man{};

} // namespace

void* cGameModeMan_getGM(void) {
    auto& gm = g_game_mode_man;
    if (gm.magic != host::kHostGameModeManMagic) {
        gm.magic = host::kHostGameModeManMagic;
    }
    return &gm;
}

int cGameModeMan_initGameMode(void* gm, int mode) {
    auto* man = host::game_mode_man_as_state(gm);
    if (!man) {
        man = host::game_mode_man_as_state(cGameModeMan_getGM());
    }
    if (!man) {
        return 0;
    }
    man->current_mode = mode;
    man->initialized = 1;

    char log_buf[80];
    std::snprintf(log_buf, sizeof(log_buf), "cGameModeMan_initGameMode mode=%d", mode);
    host::host_log("game", log_buf);
    return 1;
}

void cGameModeMan_restartHeat(void* gm) {
    auto* man = host::game_mode_man_as_state(gm);
    if (!man) {
        return;
    }
    ++man->heat_restarts;
    host::host_log("game", "cGameModeMan_restartHeat");
}

void* cGameViewMan_cGameViewMan(void* self) {
    auto* vm = host::game_view_man_as_state(self);
    if (!vm) {
        host::host_log("game", "cGameViewMan_cGameViewMan: invalid state");
        return self;
    }
    vm->frame = 0;
    host::host_log("game", "cGameViewMan_cGameViewMan (retail 0x22E3B8)");
    return self;
}

void cGameViewMan_updateAll(void* self) {
    auto* vm = host::game_view_man_as_state(self);
    if (!vm) {
        return;
    }
    ++vm->frame;
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
