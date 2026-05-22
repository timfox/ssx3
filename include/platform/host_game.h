#ifndef PLATFORM_HOST_GAME_H
#define PLATFORM_HOST_GAME_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_game_create(void);
void host_game_destroy(void* self);

void* cGame_cGame(void* self);
int cGame_load(void* self, int mode);
int cGame_loadTrack(void* self, int track_id);
void cGame_exit(void* self);
int cGameModeHandler_run(void* handler, int mode);
void host_game_tick(void* game);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_GAME_H */
