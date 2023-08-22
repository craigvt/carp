#pragma once

#include "common.h"

#define NATIVE_W 180
#define NATIVE_H 320

struct Render {
        SDL_Window *window;
        SDL_Renderer *renderer;
};

void render_init(void);
void render_destroy(void);
SDL_Renderer* render_get_renderer(void);
float render_get_scale(void);
int render_get_offset(void);