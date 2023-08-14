#include "input.h"
#include "game.h"
#include "entity.h"

static input_context ctx;

input_context *input_get_context(void)
{
        return &ctx;
}

void input_poll_events(void)
{
        game_context *game_ctx = game_get_context();
        
        while (SDL_PollEvent(&ctx.event)) { 

                if (ctx.event.type == SDL_QUIT) {
                        game_ctx->new_state = QUIT;
                        game_state_manager();
                } 
                if (ctx.event.type == SDL_FINGERDOWN) {
                        ctx.touch_loc.x = NATIVE_W * ctx.event.tfinger.x;
                        ctx.touch_loc.y = NATIVE_H * ctx.event.tfinger.y;

                        input_handle_event();                       
                }
        }
}

void input_handle_event(void)
{
        entity_context *entity_ctx = entity_get_context();

        for (uint8_t i = 0; i < MAX_FISH; i++) {
                if (entity_ctx->fish_arr[i].active) {
                        if ((ctx.touch_loc.x > entity_ctx->fish_arr[i].dst.x) && (ctx.touch_loc.x < entity_ctx->fish_arr[i].dst.x + entity_ctx->fish_arr[i].src.w +2)
                                && (ctx.touch_loc.y > entity_ctx->fish_arr[i].dst.y) && (ctx.touch_loc.y < entity_ctx->fish_arr[i].dst.y + entity_ctx->fish_arr[i].src.h +2)) {
                                        entity_ctx->fish_arr[i].dying = true;
                                        entity_ctx->fish_arr[i].death_count = 0;
                                        LOGI("Fish Touched");
                        }
                }
        }

        if ((ctx.touch_loc.x > entity_ctx->turtle.dst.x) && (ctx.touch_loc.x < entity_ctx->turtle.dst.x + entity_ctx->turtle.src.w)
                && (ctx.touch_loc.y > entity_ctx->turtle.dst.y) && (ctx.touch_loc.y < entity_ctx->turtle.dst.y + entity_ctx->turtle.src.h)) {
                        entity_ctx->turtle.dying = true;
                        LOGI("Turtle Touched");
        }

        LOGI("Event Function Completed");
}
