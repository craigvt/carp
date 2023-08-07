#pragma once

#include "common.h"

typedef struct {
        SDL_Texture *texture;
        SDL_Rect     src;
        SDL_Rect     dst;
} play_bg;

typedef struct {
        SDL_Texture *texture;
        SDL_Rect     src;
        SDL_Rect     dst;
        uint8_t      key_frame;
} bubbles;

typedef struct {
        SDL_Texture *texture;
        SDL_Rect     src;
        SDL_Rect     dst;
        uint8_t      key_frame;
        bool         active;
        uint8_t      direction;
        uint8_t      speed;
        uint16_t     timer;
} shark;

typedef struct {
        SDL_Texture *texture;
        SDL_Rect     src;
        SDL_Rect     dst;
        uint8_t      key_frame;
        bool         active;
        uint8_t      direction;
        uint8_t      speed;
        uint16_t     timer;
} school;

typedef struct {
        bool active;
        uint8_t  id;
        uint8_t  dir;
        uint8_t  speed;
        uint8_t  key_frame;
        SDL_Rect src;
        SDL_Rect dst;
} solofish;

typedef struct {
        SDL_Texture *texture;
        solofish solo_arr[MAX_BGFISH];
} solofish_grp;

typedef struct {
        play_bg  play_bg;
        bubbles  bubbles;
        shark    shark;
        school   school;
        solofish solofish;
} background_context;

background_context *background_get_context(void);
void background_init_play(void);
void background_update_play(void);
void background_render_play(void);
void background_destroy_play(void);
void background_spawn_check(void);
void background_update_bubbles(void);
void background_update_shark(void);
void background_update_school(void);