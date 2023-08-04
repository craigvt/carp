#include "render.h"

static render_context ctx;

render_context *render_get_context()
{
        return &ctx;
}

void render_init(void)
{
        /* initialize sdl, window and renderer */
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);

        ctx.window = SDL_CreateWindow("Carp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, NATIVE_W, NATIVE_H, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

        ctx.render = SDL_CreateRenderer(ctx.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        /* set render scaling */
        SDL_GetDisplayBounds(0, &ctx.bounds);
        ctx.scale_factor = (float)ctx.bounds.h / NATIVE_H;
        SDL_RenderSetScale(ctx.render, ctx.scale_factor, ctx.scale_factor);

        /* get offset width */
        ctx.scaled_width = NATIVE_W * ctx.scale_factor;

        if (ctx.scaled_width > ctx.bounds.w) {
                ctx.offset = (ctx.scaled_width - ctx.bounds.w) / 2; 
        }
        if (ctx.scaled_width < ctx.bounds.w) {
                ctx.offset = (ctx.bounds.w - ctx.scaled_width) / 2; 
        }
}

void render_destroy(void)
{
        SDL_DestroyRenderer(ctx.render);
        SDL_DestroyWindow(ctx.window);
        SDL_Quit();
}