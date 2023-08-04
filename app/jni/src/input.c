#include "input.h"
#include "game.h"

static input_context ctx;

input_context *input_get_context(void)
{
        return &ctx;
}

void input_poll_events(void)
{
        ctx.action_id = CLEAR;
        game_context *game_ctx = game_get_context();
        
        while (SDL_PollEvent(&ctx.event)) { 

                if (ctx.event.type == SDL_QUIT) {
                        game_ctx->new_state = QUIT;
                        game_state_manager();
                } 
                if (ctx.event.type == SDL_FINGERDOWN) {
                        ctx.touch_loc.x = NATIVE_W * ctx.event.tfinger.x;
                        ctx.touch_loc.y = NATIVE_H * ctx.event.tfinger.y;
                        ctx.action_id = TOUCH;
                } 
        }
}