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

        /* bg */
        ctx.play_bg.texture    = IMG_LoadTexture(render_ctx->render, "bg_play.png");
        ctx.play_bg.src        = (SDL_Rect){0, 0, PLAYBG_W, PLAYBG_H};
        ctx.play_bg.dst        = (SDL_Rect){-render_ctx->offset, 0, PLAYBG_W, PLAYBG_H};

        /* bg bubbles */
        ctx.bubbles.texture    = IMG_LoadTexture(render_ctx->render, "bg_bubbles_spritesheet.png");
        ctx.bubbles.src        = (SDL_Rect){0, 0, BUBBLES_W, BUBBLES_H};
        ctx.bubbles.dst        = (SDL_Rect){BUBBLES1_X, BUBBLES1_Y, BUBBLES_W, BUBBLES_H};
        ctx.bubbles.key_frame  = game_ctx->key_frame;

        /* bg shark */
        ctx.shark.texture      = IMG_LoadTexture(render_ctx->render, "bg_shark_spritesheet.png");
        ctx.shark.src          = (SDL_Rect){0, 0, SHARK_W, SHARK_H};
        ctx.shark.dst          = (SDL_Rect){0, 0, SHARK_W, SHARK_H};

        /* set spawn status */
        ctx.shark.timer  = 0;
        ctx.shark.active = false;
}

void background_update_bubbles(void)
{
        game_context *game_ctx = game_get_context();

        if (game_ctx->key_frame > ctx.bubbles.key_frame | game_ctx->key_frame < ctx.bubbles.key_frame) {
                if (game_ctx->key_frame < 5) {
                        ctx.bubbles.src.x = ctx.bubbles.src.x + ctx.bubbles.src.w;
                        ctx.bubbles.key_frame = game_ctx->key_frame;
                }
                if (game_ctx->key_frame == 1) {
                        ctx.bubbles.src.x = 0;
                        ctx.bubbles.key_frame = game_ctx->key_frame;
                }                              
        }
}

void background_update_shark(void)
{
        game_context *game_ctx = game_get_context();

        if (game_ctx->key_frame > ctx.shark.key_frame | game_ctx->key_frame < ctx.shark.key_frame) {
                if (game_ctx->key_frame < 5) {
                        ctx.shark.src.x = ctx.shark.src.x + ctx.shark.src.w;
                        ctx.shark.key_frame = game_ctx->key_frame;
                }
                if (game_ctx->key_frame == 1) {
                        ctx.shark.src.x  = 0;
                        ctx.shark.key_frame = game_ctx->key_frame;
                }                              
        }

        if (ctx.shark.direction == 0) {

                ctx.shark.dst.x = ctx.shark.dst.x + ctx.shark.speed;

                if (ctx.shark.dst.x > PLAYBG_W) {
                        ctx.shark.active = false;
                        ctx.shark.timer  = 0;

                        LOGI("Shark Destroyed");
                }
        }

        if (ctx.shark.direction == 1) {

                ctx.shark.dst.x = ctx.shark.dst.x - ctx.shark.speed;

                if (ctx.shark.dst.x < 0 - SHARK_W) {
                        ctx.shark.active = false;
                        ctx.shark.timer  = 0;

                        LOGI("Shark Destroyed");
                }
        }
}

void background_spawn_check(void)
{
        game_context *game_ctx = game_get_context();
       
        if (!ctx.shark.active) {
                if (ctx.shark.timer >= 600) {

                        ctx.shark.direction = rand() % 2;
                        ctx.shark.speed     = rand() % (5 + 1 - 1) + 1;
                        ctx.shark.dst.y     = rand() % (2000 + 1 - 50) + 50;
                        ctx.shark.key_frame = game_ctx->key_frame;

                        if (ctx.shark.direction == 0) {
                                ctx.shark.src.x = 0;
                                ctx.shark.src.y = 0;
                                ctx.shark.dst.x = -SHARK_W;
                        }
                        if (ctx.shark.direction == 1) {
                                ctx.shark.src.x = 0;
                                ctx.shark.src.y = SHARK_H;
                                ctx.shark.dst.x = PLAYBG_W + ctx.shark.src.w;
                        }

                        ctx.shark.active = true;

                        LOGI("Shark Spawned");
                }
        }
}

void background_update_play(void)
{
        background_update_bubbles();
        background_spawn_check();

        if (ctx.shark.active) {
                background_update_shark();
        }

        ctx.shark.timer++;
}

void background_render_play(void)
{
        render_context *render_ctx = render_get_context();

        /* bg */
        SDL_RenderCopy(render_ctx->render, ctx.play_bg.texture, &ctx.play_bg.src, &ctx.play_bg.dst);

        /* bubbles */
        SDL_RenderCopy(render_ctx->render, ctx.bubbles.texture, &ctx.bubbles.src, &ctx.bubbles.dst);

        /* shark */
        if (ctx.shark.active) {
               SDL_RenderCopy(render_ctx->render, ctx.shark.texture, &ctx.shark.src, &ctx.shark.dst); 
        }
}

void background_destroy_play(void)
{
        /* bg */
        SDL_DestroyTexture(ctx.play_bg.texture);
        ctx.play_bg.src = (SDL_Rect){0, 0, 0, 0};
        ctx.play_bg.dst = (SDL_Rect){0, 0, 0, 0};

        /* bg bubbles */
        SDL_DestroyTexture(ctx.bubbles.texture);
        ctx.bubbles.src = (SDL_Rect){0, 0, 0, 0};
        ctx.bubbles.dst = (SDL_Rect){0, 0, 0, 0};

        /* bg shark */
        SDL_DestroyTexture(ctx.shark.texture);
        ctx.shark.src = (SDL_Rect){0, 0, 0, 0};
        ctx.shark.dst = (SDL_Rect){0, 0, 0, 0};
}