#include "render.h"

static struct Render render;

SDL_Renderer *render_get_renderer(void)
{
        return render.renderer;
}

void render_draw(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst)
{
        SDL_RenderCopy(render.renderer, texture, src, dst);
}

void render_init(void)
{
        render.window = SDL_CreateWindow("Carp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, NATIVE_W, NATIVE_H, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
        render.renderer = SDL_CreateRenderer(render.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

        SDL_GetDisplayBounds(0, &render.bounds);
}

int render_get_w(void)
{
        return render.bounds.w;
}

int render_get_h(void)
{
        return render.bounds.h;
}

int render_get_x(void)
{
        return render.bounds.x;
}

int render_get_y(void)
{
        return render.bounds.y;
}

float render_get_scale(void)
{
        float scale = (float)render.bounds.h / NATIVE_H;

        return scale;
}

int render_get_offset(void)
{
        int offset;
        int scaled_width = NATIVE_W * render_get_scale();

        if (scaled_width > render.bounds.w) {
                offset = (scaled_width - render.bounds.w) / 2; 
        }
        if (scaled_width < render.bounds.w) {
                offset = (render.bounds.w - scaled_width) / 2; 
        }
        
        return offset;
}

void render_destroy(void)
{
        SDL_DestroyRenderer(render.renderer);
        SDL_DestroyWindow(render.window);
}