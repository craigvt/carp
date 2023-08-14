#pragma once

#include "common.h"

typedef struct {
        SDL_Window   *window;
        SDL_Renderer *render;
        SDL_Rect      bounds;
        float         scale_factor;
        float         scaled_x;
        int           scaled_width;
        int           offset;
} render_context;

render_context *render_get_context(void);
void render_init(void);
void render_destroy(void);