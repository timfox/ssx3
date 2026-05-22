#ifndef PLATFORM_HOST_SYSCALLS_H
#define PLATFORM_HOST_SYSCALLS_H

/*
 * Host HAL: PS2/EA symbols implemented for ssx3-native (-DSSX3_HOST).
 * Game code should call these names; definitions live under src/platform/linux/.
 *
 * When a symbol is decompiled in src/, remove the host stub and link the game TU instead.
 */

/* --- Boot / init (retail VMA in config/symbol_addrs.txt) --- */
/* main                 0x0031AF80  host_boot.cpp (C chain until decomp) */
/* __sti__all_in_one     0x0031AF50  host_boot.cpp retail_run_static_init */
/* retail preinit        0x0040FC90  host_boot.cpp retail_preinit */
/* systemInit            0x0031ADB0  host_abi.h */
/* cMemMan_initialize    0x00319A30  host_mem.cpp */
/* cMemMan_alloc         0x00317D70  host_mem.cpp */
/* cMemMan_free          0x00317E98  host_mem.cpp */
/* cExecutionMan_halt    (stub)      host_boot.cpp */
/* cAppMan_run           0x00316D60  src/dcd10/cAppMan.cpp */
/* cAppMan_mainLoop      0x00316F00  src/dcd10/cAppMan_mainLoop.cpp */
/* cFELocale_addFile     0x001959A8  src/fe/cfelocale_port.cpp */
/* cSSXApp_*             0x00226830  src/main/ssxapp_port.cpp */

/* --- Sync / time --- */
/* SYNCTASK_yieldticks   host_sync.cpp */
/* THREAD_yieldticks     host_sync.cpp */

/* --- Disc / archives --- */
/* FILESYS_*             host_filesys.cpp */
/* BIG_*                 host_big.cpp */
/* cBigFile_*            src/bx/bigfile.cpp (native decomp tree) */
/* host::read_*          host_io.cpp */

/* --- Input --- */
/* cInputPad_*           src/input/inputpad.cpp */
/* cInputMap_*           src/input/inputmap.cpp */
/* cSSXApp_loadInputMap  0x00226B60  src/main/ssxapp_port.cpp */

/* --- Graphics (stubs → Vulkan via host_gs) --- */
/* host::gs_*            host_gs.cpp */
/* cPSPGraphicsMan_New*TexID     host_graphics.cpp */
/* cRenderStateMan_SnowFlake*     host_graphics.cpp */
/* cVisualEffectsMainMenu_*       host_graphics.cpp */

/* --- FE / title (host HAL → Vulkan until FE decomp) --- */
/* cFEStateTitle_*               0x00194778  src/fe/cfestate_title.cpp */
/* cFEStateMainMenu_*            0x00194AA8  src/fe/cfestate_mainmenu.cpp */
/* cFEStateMountainRoom_*        0x001D2990  src/fe/cfestate_mountainroom.cpp */
/* cFEStateOptions_*             0x00188800  src/fe/cfestate_options.cpp */
/* cFEStateOptionsGame_*         0x00188F28  src/fe/cfestate_options_game.cpp */
/* cFEStateOptionsSound_*        0x0018A298  src/fe/cfestate_options_sound.cpp */
/* cFEStateOptionsController_*   0x0018C6A0  src/fe/cfestate_options_controller.cpp */
/* cFEStateOptionsSaveLoad_*     0x0018D280  src/fe/cfestate_options_saveload.cpp */
/* cFEStateTransition_*          0x00194588  src/fe/cfestate_transition.cpp */
/* cFEStateBonusMaterial_*       0x001954D8  src/fe/cfestate_bonus_material.cpp */
/* cFEStateEventSelect_*         0x00186658  src/fe/cfestate_event_select.cpp */
/* cFEStateSelectMultiplayerMode_* 0x001A3090  src/fe/cfestate_multimode.cpp */
/* cFEStatePeakRoom_*            0x001D3890  src/fe/cfestate_peak_room.cpp */
/* cFEStateOnlineMainMenu_*      (stub)      src/fe/cfestate_online_mainmenu.cpp */
/* cUIEngine_loadFile            0x00397C40  src/ui/uiengine_port.cpp */
/* cUIEngine_addScreenByHashName 0x00397CC0  src/ui/uiengine_port.cpp */
/* cWorld_* / cWorldView_*       (native)    src/world/world_native.cpp */
/* cWorldCache_* / cStreamMan /  (ObjDiff)   src/mem/units/rom21_*.c */
/*   cWorldSphTree */
/* early cGame_* / cGameViewMan  (ObjDiff)   src/mem/units/early_cGame*.c */
/*   cGameModeMan / cGameComm */
/* early cSSXApp tick chain      (ObjDiff)   src/mem/units/early_cSSXApp_*.c */
/* rom21 cPSPGraphicsMan tex IDs (ObjDiff)   src/mem/units/rom21_cPSPGraphicsMan_*.c */
/* early cRider_* / cRiderMetrix (ObjDiff)   src/mem/units/early_cRider*.c */
/* early cAI_* / cAirPredictor   (ObjDiff)   src/mem/units/early_cAI*.c, early_cAirPredictor_*.c */
/* dcd10 cRiderAnimBase /        (ObjDiff)   src/mem/units/dcd10_*.c */
/*   cWorldLightMan_initLightCache */
/* early cWorldPainter/Trigger   (ObjDiff)   src/mem/units/early_cWorld*.c */
/* early_cSectionMan_setSky      (ObjDiff)   src/mem/units/early_cSectionMan_setSky.c */
/* cGameModeMan_* / cGameViewMan (native)    game_mode_native.cpp */
/* cSectionMan_setSky            (native)    section_native.cpp */
/* cWorldCache_activateSectionMem(native)    world_native.cpp */
/* host_game_tick                (native)    game_native.cpp — race loop from cSSXApp_preUpdate */
/* cRider_initOnce               (decomp)    src/ai/rider.cpp + host_rider.cpp */
/* host_boot_start_race_load     (HAL)       host_boot.cpp → --boot-then-game */
/* host_world_*                  (HAL)       src/platform/linux/host_world.cpp */
/* cGame_load                    0x0022EBC8  src/main/game_native.cpp */
/* cGame_loadTrack               0x0022F6B0  src/main/game_native.cpp */
/* cSSXApp_startGameLoad         0x002281D8  src/main/ssxapp_port.cpp */
/* main / boot chain             0x0031AF80  src/main/main_native.cpp → host_boot */
/* cVisualEffectsMainMenu_*      0x0024D2F0  host_graphics.cpp + host_visualfx_menu.cpp */
/* host_menu_ensure_assets       host_menu_assets.cpp (fe_1.ssh → PNG) */
/* host_menu_audio_*             host_menu_audio.cpp (audio.big → WAV, OpenAL) */

/* --- Graphics (goal: HAL rewrite) --- */
/* cPSPGraphicsMan_*     (not wired yet) */

#endif /* PLATFORM_HOST_SYSCALLS_H */
