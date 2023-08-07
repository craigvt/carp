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

        /* bubbles */
        ctx.bubbles.texture    = IMG_LoadTexture(render_ctx->render, "bg_bubbles_spritesheet.png");
        ctx.bubbles.src        = (SDL_Rect){0, 0, BUBBLES_W, BUBBLES_H};
        ctx.bubbles.dst        = (SDL_Rect){BUBBLES_X, BUBBLES_Y, BUBBLES_W, BUBBLES_H};
        ctx.bubbles.key_frame  = game_ctx->key_frame;

        /* shark */
        ctx.shark.texture = IMG_LoadTexture(render_ctx->render, "bg_shark_spritesheet.png");
        SDL_SetTextureAlphaMod(ctx.shark.texture, 140);
        SDL_SetTextureBlendMode(ctx.shark.texture, SDL_BLENDMODE_BLEND);
        ctx.shark.src          = (SDL_Rect){0, 0, SHARK_W, SHARK_H};
        ctx.shark.dst          = (SDL_Rect){0, 0, SHARK_W, SHARK_H};
        ctx.shark.timer        = 0;
        ctx.shark.active       = false;

        /* school of fish */
        ctx.school.texture = IMG_LoadTexture(render_ctx->render, "bg_school_spritesheet.png");
        SDL_SetTextureAlphaMod(ctx.school.texture, 140);
        SDL_SetTextureBlendMode(ctx.school.texture, SDL_BLENDMODE_BLEND);
        ctx.school.src         = (SDL_Rect){0, 0, SCHOOL_W, SCHOOL_H};
        ctx.school.dst         = (SDL_Rect){0, 0, SCHOOL_W, SCHOOL_H};
        ctx.school.timer       = 0;
        ctx.school.active      = false;

        /* solo fish */
        
}

void background_update_play(void)
{
        background_update_bubbles();
        background_spawn_check();

        if (ctx.shark.active) {
                background_update_shark();
        }

        if (ctx.school.active) {
                background_update_school();
        }

        ctx.shark.timer++;
        ctx.school.timer++;
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

        /* school of fish */
        if (ctx.school.active) {
                SDL_RenderCopy(render_ctx->render, ctx.school.texture, &ctx.school.src, &ctx.school.dst);
        }
}

void background_destroy_play(void)
{
        /* bg */
        SDL_DestroyTexture(ctx.play_bg.texture);
        ctx.play_bg.src = (SDL_Rect){0, 0, 0, 0};
        ctx.play_bg.dst = (SDL_Rect){0, 0, 0, 0};

        /* bubbles */
        SDL_DestroyTexture(ctx.bubbles.texture);
        ctx.bubbles.src = (SDL_Rect){0, 0, 0, 0};
        ctx.bubbles.dst = (SDL_Rect){0, 0, 0, 0};

        /* shark */
        SDL_DestroyTexture(ctx.shark.texture);
        ctx.shark.src = (SDL_Rect){0, 0, 0, 0};
        ctx.shark.dst = (SDL_Rect){0, 0, 0, 0};

        /* school of fish */
        SDL_DestroyTexture(ctx.school.texture);
        ctx.school.src = (SDL_Rect){0, 0, 0, 0};
        ctx.school.dst = (SDL_Rect){0, 0, 0, 0};
}

void background_spawn_check(void)
{
        game_context *game_ctx = game_get_context();
       
        /* shark */
        if (!ctx.shark.active && !ctx.school.active) {
                if (ctx.shark.timer >= 1800) {

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

        /* school of fish */
        if (!ctx.school.active && !ctx.shark.active) {
                if (ctx.school.timer >= 600) {

                        ctx.school.direction = rand() % 2;
                        ctx.school.speed     = rand() % (5 + 1 - 1) + 1;
                        ctx.school.dst.y     = rand() % (2000 + 1 - 50) + 50;
                        ctx.school.key_frame = game_ctx->key_frame;

                        if (ctx.school.direction == 0) {
                                ctx.school.src.x = 0;
                                ctx.school.src.y = 0;
                                ctx.school.dst.x = -SCHOOL_W;
                        }
                        if (ctx.school.direction == 1) {
                                ctx.school.src.x = 0;
                                ctx.school.src.y = SCHOOL_H;
                                ctx.school.dst.x = PLAYBG_W + ctx.school.src.w;
                        }

                        ctx.school.active = true;

                        LOGI("School of Fish Spawned");
                }
        }
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

void background_update_school(void)
{
        game_context *game_ctx = game_get_context();

        if (game_ctx->key_frame > ctx.school.key_frame | game_ctx->key_frame < ctx.school.key_frame) {
                if (game_ctx->key_frame < 5) {
                        ctx.school.src.x = ctx.school.src.x + ctx.school.src.w;
                        ctx.school.key_frame = game_ctx->key_frame;
                }
                if (game_ctx->key_frame == 1) {
                        ctx.school.src.x  = 0;
                        ctx.school.key_frame = game_ctx->key_frame;
                }                              
        }

        if (ctx.school.direction == 0) {

                ctx.school.dst.x = ctx.school.dst.x + ctx.school.speed;

                if (ctx.school.dst.x > PLAYBG_W) {
                        ctx.school.active = false;
                        ctx.school.timer  = 0;

                        LOGI("School of Fish Destroyed");
                }
        }

        if (ctx.school.direction == 1) {

                ctx.school.dst.x = ctx.school.dst.x - ctx.school.speed;

                if (ctx.school.dst.x < 0 - SCHOOL_W) {
                        ctx.school.active = false;
                        ctx.school.timer  = 0;

                        LOGI("School of Fish Destroyed");
                }
        }
}