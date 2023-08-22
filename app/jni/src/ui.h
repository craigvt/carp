#pragma once

#include "common.h"

#define LOGO_W 112
#define LOGO_H 48
#define TITLE_W 144
#define TITLE_H 96
#define SCORE_W 180
#define SCORE_H 32
#define GAMEOVER_W 128
#define GAMEOVER_H 32
#define PLAYBTN_W 80
#define PLAYBTN_H 32
#define GO_UI_W 80
#define GO_UI_H 32

struct Ui {

        struct logo {
                SDL_Rect src;
                SDL_Rect dst;
                int frame_count;
        } logo;

        struct title {
                SDL_Rect src;
                SDL_Rect dst;
        } title;

        struct score {
                TTF_Font    *font;
                SDL_Texture *label;
                SDL_Rect     label_rect;
                SDL_Rect     score_rect;
        } score;

        struct gameover {
                SDL_Rect src;
                SDL_Rect dst;
        } gameover;

        struct play_btn {
                SDL_Rect src;
                SDL_Rect dst;
        } play_btn;

        bool score_ready;
        int last_fish_caught;
        SDL_Texture *texture;
        SDL_Rect src;
        SDL_Rect dst;
};

void ui_init(void);
void ui_update(void);
void ui_render(void);
void ui_destroy(void);
void ui_update_score(void);