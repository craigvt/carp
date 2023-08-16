#include "background.h"
#include "render.h"
#include "game.h"

static background_context ctx;

background_context *background_get_context(void)
{
        return &ctx;
}

void background_init_play(void)
{
        render_context *render_ctx = render_get_context();
        game_context   *game_ctx   = game_get_context();

        float scale = render_ctx->scale_factor;

        /* load textures */
        ctx.play_bg.texture = IMG_LoadTexture(render_ctx->render, "play_bg.png");
        ctx.texture = IMG_LoadTexture(render_ctx->render, "bg_atlas.png");

        /* bg */
        ctx.play_bg.src   = (SDL_Rect){0, 0, PLAYBG_W, PLAYBG_H};
        ctx.play_bg.dst   = (SDL_Rect){0, 0, PLAYBG_W * scale, PLAYBG_H * scale};

        /* school 1 */
        ctx.school1.src   = (SDL_Rect){0, 0, SCHOOL_W, SCHOOL_H};
        ctx.school1.dst   = (SDL_Rect){0 - SCHOOL_W * scale, 48 * scale, SCHOOL_W * scale, SCHOOL_H * scale};
        ctx.school1.speed = rand() % (2 + 1 - 1) + 1;

        /* school 2 */
        ctx.school2.src   = (SDL_Rect){0, SCHOOL_H, SCHOOL_W, SCHOOL_H};
        ctx.school2.dst   = (SDL_Rect){PLAYBG_W * scale, 128 * scale, SCHOOL_W * scale, SCHOOL_H * scale};
        ctx.school2.speed = rand() % (2 + 1 - 1) + 1;

        /* group 1 */
        ctx.fish_group1.src   = (SDL_Rect){0, 96, GROUP_W, GROUP_H};
        ctx.fish_group1.dst   = (SDL_Rect){0 - GROUP_W * scale, 112 * scale, GROUP_W * scale, GROUP_H * scale};
        ctx.fish_group1.speed = rand() % (3 + 1 - 1) + 1;

        /* group 2 */
        ctx.fish_group2.src   = (SDL_Rect){0, 112, GROUP_W, GROUP_H};
        ctx.fish_group2.dst   = (SDL_Rect){PLAYBG_W * scale, 192 * scale, GROUP_W * scale, GROUP_H * scale};
        ctx.fish_group2.speed = rand() % (3 + 1 - 1) + 1;

        /* solo 1 */
        ctx.fish_solo1.src   = (SDL_Rect){64, 96, SOLO_W, SOLO_H};
        ctx.fish_solo1.dst   = (SDL_Rect){0 - SOLO_W * scale, 96 * scale, SOLO_W * scale, SOLO_H * scale};
        ctx.fish_solo1.speed = rand() % (4 + 1 - 1) + 1;

        /* solo 2 */
        ctx.fish_solo2.src   = (SDL_Rect){64, 96, SOLO_W, SOLO_H};
        ctx.fish_solo2.dst   = (SDL_Rect){0 - SOLO_W * scale, 176 * scale, SOLO_W * scale, SOLO_H * scale};
        ctx.fish_solo2.speed = rand() % (4 + 1 - 1) + 1;

        /* solo 3 */
        ctx.fish_solo3.src   = (SDL_Rect){64, 112, SOLO_W, SOLO_H};
        ctx.fish_solo3.dst   = (SDL_Rect){PLAYBG_W * scale, 208 * scale, SOLO_W * scale, SOLO_H * scale};
        ctx.fish_solo3.speed = rand() % (4 + 1 - 1) + 1;
}

void background_update_play(void)
{
        game_context *game_ctx = game_get_context();
        render_context *render_ctx = render_get_context();

        float scale = render_ctx->scale_factor;

        ctx.school1.dst.x = ctx.school1.dst.x + ctx.school1.speed;
        if (ctx.school1.dst.x > PLAYBG_W * scale) {
                ctx.school1.dst.x = 0 - SCHOOL_W * scale;
        } 

        ctx.school2.dst.x = ctx.school2.dst.x - ctx.school2.speed;      
        if (ctx.school2.dst.x < 0 - SCHOOL_W * scale) {
                ctx.school2.dst.x = PLAYBG_W * scale;
        } 

        ctx.fish_group1.dst.x = ctx.fish_group1.dst.x + ctx.fish_group1.speed;
        if (ctx.fish_group1.dst.x > PLAYBG_W * scale) {
                ctx.fish_group1.dst.x = 0 - GROUP_W * scale;
                ctx.fish_group1.speed = rand() % (2 + 1 - 1) + 1;
        }

        ctx.fish_group2.dst.x = ctx.fish_group2.dst.x - ctx.fish_group2.speed;
        if (ctx.fish_group2.dst.x < 0 - GROUP_W * scale) {
                ctx.fish_group2.dst.x = PLAYBG_W * scale;
                ctx.fish_group2.speed = rand() % (2 + 1 - 1) + 1;
        }

        ctx.fish_solo1.dst.x = ctx.fish_solo1.dst.x + ctx.fish_solo1.speed;
        if (ctx.fish_solo1.dst.x > PLAYBG_W * scale) {
                ctx.fish_solo1.dst.x = 0 - SOLO_W * scale;
                ctx.fish_solo1.speed = rand() % (3 + 1 - 1) + 1;
        }

        ctx.fish_solo2.dst.x = ctx.fish_solo2.dst.x + ctx.fish_solo2.speed;
        if (ctx.fish_solo2.dst.x > PLAYBG_W * scale) {
                ctx.fish_solo2.dst.x = 0 - SOLO_W * scale;
                ctx.fish_solo2.speed = rand() % (3 + 1 - 1) + 1;
        }

        ctx.fish_solo3.dst.x = ctx.fish_solo3.dst.x - ctx.fish_solo3.speed;
        if (ctx.fish_solo3.dst.x < 0 - SOLO_W * scale) {
                ctx.fish_solo3.dst.x = 0 - SOLO_W * scale;
                ctx.fish_solo3.speed = rand() % (3 + 1 - 1) + 1;
        }

        if (game_ctx->key_frame == 1) {
                ctx.school1.src.x = 0;
                ctx.school2.src.x = 0;
                ctx.fish_group1.src.x = 0;
                ctx.fish_group2.src.x = 0;
                ctx.fish_solo1.src.x  = 64;
                ctx.fish_solo2.src.x  = 64;
                ctx.fish_solo3.src.x  = 64;
        }

        if (game_ctx->key_frame == 2) {
                ctx.school1.src.x = SCHOOL_W;
                ctx.school2.src.x = SCHOOL_W;
                ctx.fish_group1.src.x = GROUP_W;
                ctx.fish_group2.src.x = GROUP_W;
                ctx.fish_solo1.src.x  = 64 + SOLO_W;
                ctx.fish_solo2.src.x  = 64 + SOLO_W;
                ctx.fish_solo3.src.x  = 64 + SOLO_W;
        }                              
}

void background_render_play(void)
{
        render_context *render_ctx = render_get_context();

        SDL_RenderCopy(render_ctx->render, ctx.play_bg.texture, &ctx.play_bg.src, &ctx.play_bg.dst);
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.school1.src, &ctx.school1.dst);
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.school2.src, &ctx.school2.dst);
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.fish_group1.src, &ctx.fish_group1.dst);
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.fish_group2.src, &ctx.fish_group2.dst);
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.fish_solo1.src, &ctx.fish_solo1.dst);
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.fish_solo2.src, &ctx.fish_solo2.dst);
        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.fish_solo3.src, &ctx.fish_solo3.dst);
}

void background_destroy_play(void)
{
        SDL_DestroyTexture(ctx.play_bg.texture);
        SDL_DestroyTexture(ctx.texture);
        ctx.play_bg.src     = (SDL_Rect){0, 0, 0, 0};
        ctx.play_bg.dst     = (SDL_Rect){0, 0, 0, 0};
        ctx.school1.src     = (SDL_Rect){0, 0, 0, 0};
        ctx.school1.dst     = (SDL_Rect){0, 0, 0, 0};
        ctx.school2.src     = (SDL_Rect){0, 0, 0, 0};
        ctx.school2.dst     = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_group1.src = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_group1.dst = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_group2.src = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_group2.dst = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_solo1.src  = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_solo1.dst  = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_solo2.src  = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_solo2.dst  = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_solo3.src  = (SDL_Rect){0, 0, 0, 0};
        ctx.fish_solo3.dst  = (SDL_Rect){0, 0, 0, 0};
}
