#pragma once

#include "common.h"

#define PLAYBG_W 244
#define PLAYBG_H 320
#define SCHOOL_W 64
#define SCHOOL_H 48
#define GROUP_W  32
#define GROUP_H  16
#define SOLO_W   16
#define SOLO_H   16

struct Background {

        SDL_Texture *texture;

        struct play_bg {
                SDL_Texture *texture;
                SDL_Rect     src;
                SDL_Rect     dst;
        } play_bg;

        struct school1 {
                SDL_Rect src;
                SDL_Rect dst;
                uint8_t  speed;
        } school1;

        struct school2 {
                SDL_Rect src;
                SDL_Rect dst;
                uint8_t  speed;
        } school2;

        struct fish_group1 {
                SDL_Rect src;
                SDL_Rect dst;
                uint8_t  speed;
        } fish_group1;

        struct fish_group2 {
                SDL_Rect src;
                SDL_Rect dst;
                uint8_t  speed;
        } fish_group2;

        struct fish_solo1 {
                SDL_Rect src;
                SDL_Rect dst;
                uint8_t  speed;
        } fish_solo1;

        struct fish_solo2 {
                SDL_Rect src;
                SDL_Rect dst;
                uint8_t  speed;
        } fish_solo2;

        struct fish_solo3 {
                SDL_Rect src;
                SDL_Rect dst;
                uint8_t  speed;
        } fish_solo3;
};

void background_init_play(void);
void background_update_play(void);
void background_render_play(void);
void background_destroy_play(void);