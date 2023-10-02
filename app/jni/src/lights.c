#include "lights.h"
#include "render.h"
#include "game.h"

static struct Lights lights;

void lights_init(void)
{
    float scale = render_get_scale();
    SDL_Rect bounds;
    SDL_GetDisplayBounds(0, &bounds);

    // title light
    lights.title_light.texture = IMG_LoadTexture(render_get_renderer(), "light64.png");
    lights.title_light.src = (SDL_Rect){0, 0, 64, 64};
    lights.title_light.dst = (SDL_Rect){4, 6 * scale, 192 * scale, 192 * scale};
    SDL_SetTextureBlendMode(lights.title_light.texture, SDL_BLENDMODE_ADD);
    SDL_SetTextureAlphaMod(lights.title_light.texture, 220);

    // title shadow map
    lights.title_shadow.texture = IMG_LoadTexture(render_get_renderer(), "title_shadowmap.png");
    SDL_SetTextureBlendMode(lights.title_shadow.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(lights.title_shadow.texture, 65);
}

void lights_render(void)
{
    if (game_get_state() == TITLE) {
        SDL_RenderCopy(render_get_renderer(), lights.title_light.texture, &lights.title_light.src, &lights.title_light.dst);
        SDL_RenderCopy(render_get_renderer(), lights.title_shadow.texture, NULL, NULL);
    }
}

void lights_destroy(void)
{
    SDL_DestroyTexture(lights.title_light.texture);
    SDL_DestroyTexture(lights.title_shadow.texture);
}