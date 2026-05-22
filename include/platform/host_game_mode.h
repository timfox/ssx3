#ifndef PLATFORM_HOST_GAME_MODE_H
#define PLATFORM_HOST_GAME_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_game_view_create(void);
void host_game_view_destroy(void* self);

void* cGameModeMan_getGM(void);
int cGameModeMan_initGameMode(void* gm, int mode);
void cGameModeMan_restartHeat(void* gm);
void* cGameViewMan_cGameViewMan(void* self);
void cGameViewMan_updateAll(void* self);

void host_game_tick(void* game);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_GAME_MODE_H */
