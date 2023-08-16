#pragma once

#include "common.h"

#define LANES      14
#define MAX_FISH   20
#define BIGFISH_W  32
#define SMOLFISH_W 16
#define FISH_H     16
#define TURTLE_W   16
#define TURTLE_H   16

typedef struct {
        bool active;
        bool dying;
        uint8_t  death_count;
        uint8_t  id;
        uint8_t  type;
        uint8_t  direction;
        uint8_t  speed;
        uint8_t  lane;
        SDL_Rect src;
        SDL_Rect dst;
} fish;

typedef struct {
        uint8_t  direction;
        uint8_t  speed;
        uint8_t  lane;
        SDL_Rect src;
        SDL_Rect dst;
        uint8_t  death_count;
        bool     dying;
} turtle;

typedef struct {
        uint8_t id;
        bool    active;
} lane;

typedef struct {
        SDL_Texture *texture;
        lane        lanes[LANES];
        fish        fish_arr[MAX_FISH];
        uint8_t     fish_active;
        uint8_t     fish_cap;
        uint8_t     fish_spawn_timer;
        uint8_t     fish_spawn_rate;
        uint16_t    fish_caught;
        turtle      turtle;
        bool        turtle_active;
        uint8_t     turtles_caught;
        uint8_t     turtle_spawn_timer;
        uint8_t     turtle_spawn_rate;
} entity_context;

entity_context *entity_get_context(void);
void entity_init_play(void);
void entity_update_play(void);
void entity_render_play(void);
void entity_destroy_play(void);
void entity_spawn_fish(void);
void entity_spawn_turtle(void);
void entity_update_fish(void);
void entity_update_turtle(void);
void entity_turtle_randomizer(int *spawn_y);
void entity_fish_randomizer(int *spawn_y, uint8_t *lane);
