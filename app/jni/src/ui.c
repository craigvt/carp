#include "ui.h"
#include "render.h"
#include "entity.h"

static ui_context ctx;
static render_context *render_ctx;
static entity_context *entity_ctx;

ui_context *ui_get_context(void)
{
        return &ctx;
}

void ui_init_play(void)
{
        render_ctx = render_get_context();
        entity_ctx = entity_get_context();

        float scale = render_ctx->scale_factor;
        uint16_t ui_offset = render_ctx->offset / render_ctx->scale_factor;

        ctx.texture = IMG_LoadTexture(render_ctx->render, "ui_atlas.png");

        /* score */
        ctx.score.src = (SDL_Rect){0, 0, SCORE_W, SCORE_H};
        ctx.score.dst = (SDL_Rect){ (16 + ui_offset) * scale, 0, SCORE_W * scale, SCORE_H * scale};

        /* life */
        ctx.life.life1_src = (SDL_Rect){0, 32, LIFE_W, LIFE_H};
        ctx.life.life1_dst = (SDL_Rect){ (79 + ui_offset) * scale, 0, LIFE_W * scale, LIFE_H * scale};
        ctx.life.life2_src = (SDL_Rect){0, 32, LIFE_W, LIFE_H};
        ctx.life.life2_dst = (SDL_Rect){ (107 + ui_offset) * scale, 0, LIFE_W * scale, LIFE_H * scale};
        ctx.life.life3_src = (SDL_Rect){0, 32, LIFE_W, LIFE_H};
        ctx.life.life3_dst = (SDL_Rect){ (135 + ui_offset) * scale, 0, LIFE_W * scale, LIFE_H * scale};

        /* initialize score keeping */
        ctx.total_life  = 3;
        ctx.last_score  = entity_ctx->fish_caught;
        ctx.score_ready = false;
}

void ui_update_play(void)
{
        if (!ctx.score_ready) {
                ui_update_score();
                ctx.score_ready = true;
        }

        if (entity_ctx->fish_caught > ctx.last_score) {
                ui_update_score();
                ctx.last_score = entity_ctx->fish_caught;
        }

        switch (entity_ctx->turtles_caught) {
                case 0: ctx.total_life = 3; break;
                case 1: ctx.total_life = 2; break;
                case 2: ctx.total_life = 1; break;
        }
}

void ui_render_play(void)
{
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.score.src, &ctx.score.dst);
        SDL_RenderCopy(render_ctx->render, ctx.score.label, NULL, &ctx.score.label_rect);

        switch (ctx.total_life) {
        case 3:
                SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.life.life1_src, &ctx.life.life1_dst);
                SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.life.life2_src, &ctx.life.life2_dst);
                SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.life.life3_src, &ctx.life.life3_dst);
                break;
        case 2:
                SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.life.life1_src, &ctx.life.life1_dst);
                SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.life.life2_src, &ctx.life.life2_dst);
                break;
        case 1:
                SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.life.life1_src, &ctx.life.life1_dst);
                break;
        }
}

void ui_destroy_play(void)
{
        SDL_DestroyTexture(ctx.texture);
        SDL_DestroyTexture(ctx.score.label);

        ctx.score.src        = (SDL_Rect){0, 0, 0, 0};
        ctx.score.dst        = (SDL_Rect){0, 0, 0, 0};
        ctx.score.label_rect = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life1_src   = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life1_dst   = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life2_src   = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life2_dst   = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life3_src   = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life3_dst   = (SDL_Rect){0, 0, 0, 0};
}

void ui_update_score(void)
{
        float scale = render_ctx->scale_factor;
        uint16_t ui_offset = render_ctx->offset / render_ctx->scale_factor;

        /* clear previous texture */
        SDL_DestroyTexture(ctx.score.label);

        /* open font */
        ctx.score.font = TTF_OpenFont("fonts/BitPap.ttf", 4 * scale);
        
        /* convert fish caught int to string */
        uint16_t score = entity_ctx->fish_caught;
        char *text;
        snprintf(text, 4, "%d", score);

        /* set the text color */
        SDL_Color black = (SDL_Color){0, 0, 0, 255};

        /* render text to surface */
        SDL_Surface *surface = TTF_RenderText_Solid(ctx.score.font, text, black);

        /* create texture from surface */
        ctx.score.label = SDL_CreateTextureFromSurface(render_ctx->render, surface);

        /* initiliaze label rect with W and H of surface */
        ctx.score.label_rect = (SDL_Rect){(55 + ui_offset) * scale, (12 * scale) - surface->h, surface->w * scale, surface->h * scale};
        LOGI("Text W: %d, Text H: %d", surface->w, surface->h);

        /* free the surface and close the font */
        SDL_FreeSurface(surface);
        TTF_CloseFont(ctx.score.font);
}