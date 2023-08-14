#include "input.h"
#include "game.h"
#include "entity.h"
#include "render.h"

static input_context ctx;

input_context *input_get_context(void)
{
        return &ctx;
}

void input_poll_events(void)
{
        game_context *game_ctx     = game_get_context();
        render_context *render_ctx = render_get_context();
     
        while (SDL_PollEvent(&ctx.event)) { 

                if (ctx.event.type == SDL_QUIT) {
                        game_ctx->new_state = QUIT;
                        game_state_manager();
                }
                
                if (ctx.event.type == SDL_FINGERDOWN) {

                        ctx.touch_loc.x = (NATIVE_W * ctx.event.tfinger.x) + render_ctx->scaled_x;
                        ctx.touch_loc.y = NATIVE_H * ctx.event.tfinger.y;

                        LOGI("Touch X: %d", ctx.touch_loc.x);
                        LOGI("Touch Y: %d", ctx.touch_loc.y);
                        LOGI("Target X: 40, Target Y: 200");

                        input_handle_event();                       
                }
        }
}

void input_handle_event(void)
{
        entity_context *entity_ctx = entity_get_context();

        SDL_bool hit;

        if (entity_ctx->turtle_active) {
                hit = SDL_PointInRect(&ctx.touch_loc, &entity_ctx->turtle.dst);
                if(hit) {
                        entity_ctx->turtle.dying = true;
                        entity_ctx->turtle.death_count = 0;
                }
        }

        for (uint8_t i = 0; i < MAX_FISH; i++) {
                if (entity_ctx->fish_arr[i].active) {
                        hit = SDL_PointInRect(&ctx.touch_loc, &entity_ctx->fish_arr[i].dst);
                        if (hit) {
                                entity_ctx->fish_arr[i].dying = true;
                                entity_ctx->fish_arr[i].death_count = 0;
                        }
                }
        } 
}
