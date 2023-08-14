#pragma once

#include "common.h"

#define SCORE_W 48
#define SCORE_H 32
#define LIFE_W  32
#define LIFE_H  32

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
} score;

typedef struct {
        SDL_Rect life1_src;
        SDL_Rect life1_dst;
        SDL_Rect life2_src;
        SDL_Rect life2_dst;
        SDL_Rect life3_src;
        SDL_Rect life3_dst;
} life;

typedef struct {
        SDL_Texture *texture;
        score score;
        life  life;
        uint8_t total_life;
} ui_context;

ui_context *ui_get_context(void);
void ui_init_play(void);
void ui_update_play(void);
void ui_render_play(void);
void ui_destroy_play(void);