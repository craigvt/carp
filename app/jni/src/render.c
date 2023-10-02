#include "render.h"

static struct Render render;

void render_init(void)
{
        render.window = SDL_CreateWindow("Carp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, NATIVE_W, NATIVE_H, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
        render.renderer = SDL_CreateRenderer(render.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        SDL_SetRenderDrawBlendMode(render.renderer, SDL_BLENDMODE_BLEND);
}

void render_destroy(void)
{
        SDL_DestroyRenderer(render.renderer);
        SDL_DestroyWindow(render.window);
}

SDL_Renderer *render_get_renderer(void)
{
        return render.renderer;
}

float render_get_scale(void)
{
        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);
        float scale = (float)bounds.h / NATIVE_H;

        return scale;
}

int render_get_offset(void)
{
        int offset;
        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);

        int scaled_width = NATIVE_W * render_get_scale();

        if (scaled_width > bounds.w) {
                offset = (scaled_width - bounds.w) / 2; 
        }
        if (scaled_width < bounds.w) {
                offset = (bounds.w - scaled_width) / 2; 
        }
        
        return offset;
}