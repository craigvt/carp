#include "ui.h"
#include "render.h"

static ui_context ctx;

ui_context *ui_get_context(void)
{
        return &ctx;
}

void ui_init_play(void)
{
        render_context *render_ctx = render_get_context();

        ctx.texture = IMG_LoadTexture(render_ctx->render, "ui_atlas.png");

        uint16_t ui_offset = render_ctx->offset / render_ctx->scale_factor;

        ctx.score.src = (SDL_Rect){0, 0, SCORE_W, SCORE_H};
        ctx.score.dst = (SDL_Rect){22 + ui_offset, 0, SCORE_W, SCORE_H};

        ctx.life.life1_src = (SDL_Rect){0, 32, LIFE_W, LIFE_H};
        ctx.life.life1_dst = (SDL_Rect){70 + ui_offset, 0, LIFE_W, LIFE_H};
        ctx.life.life2_src = (SDL_Rect){0, 32, LIFE_W, LIFE_H};
        ctx.life.life2_dst = (SDL_Rect){99 + ui_offset, 0, LIFE_W, LIFE_H};
        ctx.life.life3_src = (SDL_Rect){0, 32, LIFE_W, LIFE_H};
        ctx.life.life3_dst = (SDL_Rect){128 + ui_offset, 0, LIFE_W, LIFE_H};

        ctx.total_life = 3;
}

void ui_update_play(void)
{

}

void ui_render_play(void)
{
        render_context *render_ctx = render_get_context();

        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.score.src, &ctx.score.dst);

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

        ctx.score.src      = (SDL_Rect){0, 0, 0, 0};
        ctx.score.dst      = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life1_src = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life1_dst = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life2_src = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life2_dst = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life3_src = (SDL_Rect){0, 0, 0, 0};
        ctx.life.life3_dst = (SDL_Rect){0, 0, 0, 0};
}