#pragma once

#include "common.h"

#define SCHOOL_W 64
#define SCHOOL_H 48
#define GROUP_W  32
#define GROUP_H  16
#define SOLO_W   16
#define SOLO_H   16

typedef struct {
        SDL_Texture *texture;
        SDL_Rect     src;
        SDL_Rect     dst;
} play_bg;

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  speed;
} school1;

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  speed;
} school2;

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  speed;
} fish_group1;

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  speed;
} fish_group2;

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  speed;
} fish_solo1;

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  speed;
} fish_solo2;

typedef struct {
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  speed;
} fish_solo3;

typedef struct {
        SDL_Texture *texture;
        play_bg      play_bg;
        school1      school1;
        school2      school2;
        fish_group1  fish_group1;
        fish_group2  fish_group2;
        fish_solo1   fish_solo1;
        fish_solo2   fish_solo2;
        fish_solo3   fish_solo3;
} background_context;

background_context *background_get_context(void);
void background_init_play(void);
void background_update_play(void);
void background_render_play(void);
void background_destroy_play(void);