#include "game.h"
#include "render.h"
#include "input.h"
#include "background.h"
#include "entity.h"

static game_context ctx;

game_context *game_get_context(void)
{
        return &ctx;
}

void game(void)
{
        LOGI("--------------- Game Start ---------------");

        srand(time(NULL));

        render_init();
        game_init();

        while (ctx.running) {

                input_poll_events();

                game_update();

                game_render();

                game_frame_count();
        }

        render_destroy();
}

void game_init(void)
{
        ctx.running     = true;
        ctx.new_state   = PLAY;
        game_state_manager();
}

void game_state_manager(void)
{
        switch (ctx.new_state) {
                case PLAY:
                        background_init_play();
                        entity_init_fish();
                        ctx.current_state = PLAY;
                        break;
                case QUIT:
                        if (ctx.current_state == PLAY) {
                                background_destroy_play();
                                entity_destroy_fish();
                                ctx.running = false;
                        }
                        break;
        }
}

void game_update(void)
{
        switch (ctx.current_state) {
                case PLAY:
                        background_update_play();
                        entity_spawn_fish(); 
                        entity_update_fish();             
                        break;
                case QUIT:
                        break;
        }
}

void game_render(void)
{
        render_context *render_ctx = render_get_context();

        SDL_RenderClear(render_ctx->render);

        switch (ctx.current_state) {
                case PLAY:
                        background_render_play();
                        entity_render_play();         
                        break;
                case QUIT:
                        break;
        } 

        SDL_RenderPresent(render_ctx->render);
}

void game_frame_count(void)
{
        ctx.frame_count++; 

        if (ctx.frame_count >= 15) {
                if (ctx.key_frame < 5) {
                        ctx.key_frame++;
                        ctx.frame_count = 0;
                }
                if (ctx.key_frame == 5) {
                        ctx.key_frame = 1;
                        ctx.frame_count = 0;
                }
        }
}