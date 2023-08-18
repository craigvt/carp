#include "game.h"
#include "engine/system.h"
#include "engine/render.h"
#include "engine/input.h"
#include "background.h"
#include "entity.h"
#include "ui.h"

static struct Game game;

void game_loop(void)
{
        srand(time(NULL));
        render_init();
        game_init();
        game_change_state(1);

        while (game.running) {

                game.start = SDL_GetPerformanceCounter();

                input_poll_events();

                game_update();

                game_render();

                game_frame_count();

                game.end = SDL_GetPerformanceCounter();
             
                game_fps_cap();
        }

        render_destroy();
        system_quit();
}

void game_init(void)
{
        game.running = true;
        game.frame_count = 0;
        game.key_frame = 1;
}

int game_get_keyframe(void)
{
        return game.key_frame;
}

void game_change_state(int new_state)
{
        switch (new_state) {
        case 0:
                if (game.state == PLAY) {
                        background_destroy_play();
                        entity_destroy_play();
                        ui_destroy_play();
                        game.running = false;
                }
                break;
        case 1:
                background_init_play();
                entity_init_play();
                ui_init_play();
                break;
        }
}

void game_update(void)
{
        switch (game.state) {
                case PLAY:
                        background_update_play();
                        entity_update_play(); 
                        ui_update_play();         
                        break;
                case QUIT:
                        break;
        }
}

void game_render(void)
{
        SDL_RenderClear(render_get_renderer());

        switch (game.state) {
                case PLAY:
                        background_render_play();
                        entity_render_play(); 
                        ui_render_play();         
                        break;
                case QUIT:
                        break;
        }

        SDL_RenderPresent(render_get_renderer());
}

void game_frame_count(void)
{
        game.frame_count++; 

        if (game.frame_count >= 15) {

                switch (game.key_frame) {
                case 1:
                        game.key_frame = 2;
                        game.frame_count = 0;
                        break;
                case 2:
                        game.key_frame = 1;
                        game.frame_count = 0;
                        break;
                }
        }       
}

void game_fps_cap(void) 
{
        float elapsedMS = (game.end - game.start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(33.3333333333f - elapsedMS));
}