#pragma once

#include "common.h"

typedef struct {
        bool active;
        uint8_t  id;
        uint8_t  type;
        uint8_t  dir;
        uint8_t  speed;
        uint8_t  key_frame;
        SDL_Rect src;
        SDL_Rect dst;
} fish;

typedef struct {
        SDL_Texture *fish_texture;
        fish        fish_arr[MAX_FISH];
        uint8_t     fish_active;
        uint8_t     fish_cap;
        uint16_t    fish_caught;
        uint8_t     frame_count;
} entity_context;

entity_context *entity_get_context(void);
void entity_init_fish(void);
void entity_spawn_fish(void);
void entity_update_fish(void);
void entity_destroy_fish(void);
void entity_render_play(void);