#pragma once

#include "common.h"

#define NATIVE_W 180
#define NATIVE_H 320

struct Render {
        SDL_Window   *window;
        SDL_Renderer *renderer;
        SDL_Rect bounds;
};

SDL_Renderer* render_get_renderer(void);
void  render_draw(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst);
void  render_init(void);
void  render_destroy(void);
float render_get_scale(void);
int   render_get_offset(void);
int   render_get_w(void);
int   render_get_h(void);
int   render_get_x(void);
int   render_get_y(void);