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
/* cAppMan_run           0x00316D60  host_boot.cpp */
/* cAppMan_mainLoop      0x00316F00  host_boot.cpp */

/* --- Sync / time --- */
/* SYNCTASK_yieldticks   host_sync.cpp */
/* THREAD_yieldticks     host_sync.cpp */

/* --- Disc / archives --- */
/* FILESYS_*             host_filesys.cpp */
/* BIG_*                 host_big.cpp */
/* cBigFile_*            host_bigfile.cpp */
/* host::read_*          host_io.cpp */

/* --- Input --- */
/* cInputPad_*           host_pad.cpp */
/* cInputMap_*           host_input.cpp */
/* cSSXApp_loadInputMap  host_input.cpp */

/* --- Graphics (stubs → Vulkan via host_gs) --- */
/* host::gs_*            host_gs.cpp */
/* cPSPGraphicsMan_New*TexID     host_graphics.cpp */
/* cRenderStateMan_SnowFlake*     host_graphics.cpp */
/* cVisualEffectsMainMenu_*       host_graphics.cpp */

/* --- FE / title (host HAL → Vulkan until FE decomp) --- */
/* cFEStateTitle_*               0x00194778  host_fe_title.cpp */
/* cFEStateMainMenu_*            0x00194AA8  host_fe_mainmenu.cpp */
/* cFEStateMountainRoom_*        0x001D2990  host_fe_mountain_room.cpp */
/* cFEStateOptions_*             0x00188800  host_fe_options.cpp */
/* cFEStateOptionsGame_*         0x00188F28  host_fe_options_game.cpp */
/* cFEStateOptionsSound_*        0x0018A298  host_fe_options_sound.cpp */
/* cVisualEffectsMainMenu_*      0x0024D2F0  host_graphics.cpp + host_visualfx_menu.cpp */
/* host_menu_ensure_assets       host_menu_assets.cpp (fe_1.ssh → PNG) */
/* host_menu_audio_*             host_menu_audio.cpp (audio.big → WAV, OpenAL) */

/* --- Graphics (goal: HAL rewrite) --- */
/* cPSPGraphicsMan_*     (not wired yet) */

#endif /* PLATFORM_HOST_SYSCALLS_H */
