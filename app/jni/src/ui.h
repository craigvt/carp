#pragma once

#include "common.h"
#include "render.h"
#include "entity.h"

#define SCORE_W 180
#define SCORE_H 32

typedef struct {
        TTF_Font    *font;
        SDL_Texture *label;
        SDL_Rect     label_rect;
        SDL_Rect     score_rect;
} score;

typedef struct {
        SDL_Texture *texture;
        SDL_Rect     src;
        SDL_Rect     dst;
        score        score;
        bool         score_ready;
        uint16_t     fish_caught;
        uint16_t     last_fish_caught;
} ui_context;

ui_context *ui_get_context(void);
void ui_init_play(void);
void ui_update_play(void);
void ui_render_play(void);
void ui_destroy_play(void);
void ui_update_score(void);