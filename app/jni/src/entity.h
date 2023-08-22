#pragma once

#include "common.h"

#define LANES      14
#define MAX_FISH   20
#define BIGFISH_W  32
#define SMOLFISH_W 16
#define FISH_H     16
#define TURTLE_W   16
#define TURTLE_H   16

struct Entity {

        struct fish {
                int death_count;
                int id;
                int type;
                int direction;
                int speed;
                int lane;
                bool active;
                bool dying;
                SDL_Rect src;
                SDL_Rect dst;
        } fish;

        struct lane {
                int id;
                bool active;
        } lane;

        struct turtle {
                int direction;
                int speed;
                int lane;
                int death_count;
                bool dying;
                SDL_Rect src;
                SDL_Rect dst;
        } turtle;

        int fish_active;
        int fish_cap;
        int fish_spawn_timer;
        int fish_spawn_rate;
        int fish_caught;
        int turtles_caught;
        int turtle_spawn_timer;
        int turtle_spawn_rate;
        bool turtle_active;
        struct lane lanes[LANES];
        struct fish fish_arr[MAX_FISH];
        SDL_Texture *texture;
};

void entity_init(void);
void entity_update(void);
void entity_end_state(void);
void entity_render(void);
void entity_destroy(void);
void entity_spawn_fish(void);
void entity_spawn_turtle(void);
void entity_update_fish(void);
void entity_update_turtle(void);
void entity_turtle_randomizer(int *spawn_y);
void entity_fish_randomizer(int *spawn_y, int *lane);
int entity_get_fish_caught(void);
int entity_get_turtles_caught(void);

