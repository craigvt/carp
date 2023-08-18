#pragma once

#include "common.h"

#define SCORE_W 180
#define SCORE_H 32

struct Ui {

        struct score {
                TTF_Font    *font;
                SDL_Texture *label;
                SDL_Rect     label_rect;
                SDL_Rect     score_rect;
        } score;

        bool score_ready;
        int last_fish_caught;
        SDL_Texture *texture;
        SDL_Rect src;
        SDL_Rect dst;
};

void ui_init_play(void);
void ui_update_play(void);
void ui_render_play(void);
void ui_destroy_play(void);
void ui_update_score(void);