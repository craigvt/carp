#include "ui.h"
#include "render.h"
#include "entity.h"

static ui_context ctx;

ui_context *ui_get_context(void)
{
        return &ctx;
}

void ui_init_play(void)
{
        render_context *render_ctx = render_get_context();
        entity_context *entity_ctx = entity_get_context();

        float scale = render_ctx->scale_factor;

        ctx.texture = IMG_LoadTexture(render_ctx->render, "ui_atlas.png");

        ctx.src = (SDL_Rect){0, 0, SCORE_W, SCORE_H};
        ctx.dst = (SDL_Rect){render_ctx->offset, 0, SCORE_W * scale, SCORE_H * scale};

        ctx.score.score_rect = (SDL_Rect){49 * scale + render_ctx->offset, 9 * scale, 30 * scale, 16 * scale};

        ctx.last_fish_caught = entity_ctx->fish_caught;
        ctx.score_ready = false;
}

void ui_update_play(void)
{
        entity_context *entity_ctx = entity_get_context();
        ctx.fish_caught = entity_ctx->fish_caught;

        if (!ctx.score_ready) {
                ui_update_score();
                ctx.score_ready = true;
        }

        if (entity_ctx->fish_caught > ctx.last_fish_caught) {
                ui_update_score();
                ctx.last_fish_caught = entity_ctx->fish_caught;
        }

        switch (entity_ctx->turtles_caught) {
                case 0: ctx.src.y = 0; break;
                case 1: ctx.src.y = SCORE_H; break;
                case 2: ctx.src.y = SCORE_H * 2; break;
                case 3: ctx.src.y = SCORE_H * 3; break;
        }
}

void ui_render_play(void)
{
        render_context *render_ctx = render_get_context();

        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.src, &ctx.dst);
        SDL_RenderCopy(render_ctx->render, ctx.score.label, NULL, &ctx.score.label_rect);
}

void ui_destroy_play(void)
{
        SDL_DestroyTexture(ctx.texture);
        SDL_DestroyTexture(ctx.score.label);

        ctx.src = (SDL_Rect){0, 0, 0, 0};
        ctx.dst = (SDL_Rect){0, 0, 0, 0};
        ctx.score.label_rect = (SDL_Rect){0, 0, 0, 0};
}

void ui_update_score(void)
{
        render_context *render_ctx = render_get_context();
        float scale = render_ctx->scale_factor;

        SDL_DestroyTexture(ctx.score.label);

        ctx.score.font = TTF_OpenFont("fonts/BitPap.ttf", 4 * scale);
        
        char *text;
        snprintf(text, 4, "%d", ctx.fish_caught);

        SDL_Color blue = (SDL_Color){0, 120, 139, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Solid(ctx.score.font, text, blue);
        ctx.score.label = SDL_CreateTextureFromSurface(render_ctx->render, surface);

        int text_center  = surface->w * scale / 2;
        int score_center = ctx.score.score_rect.x + ctx.score.score_rect.w / 2;
        int text_pos_x   = score_center - text_center;

        ctx.score.label_rect = (SDL_Rect){text_pos_x, ctx.score.score_rect.y, surface->w * scale, surface->h * scale};

        SDL_FreeSurface(surface);
        TTF_CloseFont(ctx.score.font);
}