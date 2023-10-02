#pragma once

#include "common.h"

struct Lights {

    struct title_map {
        SDL_Texture *texture;
        SDL_Rect src;
        SDL_Rect dst;
    } title_light;

    struct title_shadow {
        SDL_Texture *texture;
    } title_shadow;

};

void lights_init(void);
void lights_render(void);
void lights_destroy(void);