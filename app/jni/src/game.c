#include "game.h"
#include "system.h"
#include "render.h"
#include "input.h"
#include "background.h"
#include "entity.h"
#include "ui.h"

static struct Game game;

void game_loop(void)
{
        srand(time(NULL));
        render_init();
        game_init();

        while (game.running) {

                game.start = SDL_GetPerformanceCounter();

                input_poll_events();

                /* update */
                background_update();
                entity_update();
                ui_update();         

                /* render */
                SDL_RenderClear(render_get_renderer());
                background_render();
                entity_render(); 
                ui_render();                     
                SDL_RenderPresent(render_get_renderer());

                /* state check */
                game_state_manager();

                /* frame lock */
                game_frame_count();           
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
        
        game_set_new_state(1);
        game_state_manager();
}

void game_state_manager(void)
{
        if (game.state != game.new_state) {

                switch (game.new_state) {

                /* QUIT */
                case 0:
                        if (game.state == PLAY) {
                                background_destroy();
                                entity_destroy();
                                ui_destroy();
                                game.running = false;
                        }
                        if (game.state == GAMEOVER) {
                                background_destroy();
                                ui_destroy();
                                game.running = false;
                        }
                        break;

                /* LOGO */
                case 1:
                        game.state = LOGO;
                        background_init();
                        ui_init();
                        break;

                /* TITLE */
                case 2:
                        if (game.state == LOGO) {
                                background_destroy();
                                ui_destroy();
                        }
                        game.state = TITLE;
                        background_init();
                        ui_init();
                        break;

                /* PLAY */
                case 3:
                        if (game.state == GAMEOVER) {
                                background_destroy();
                                ui_destroy();
                        }
                        if (game.state == TITLE) {
                                background_destroy();
                                ui_destroy();
                        }
                        game.state = PLAY;
                        background_init();
                        entity_init();
                        ui_init();
                        break;

                /* GAMEOVER */
                case 4:
                        if (game.state == PLAY) {
                                background_destroy();
                                entity_destroy();
                                ui_destroy();
                        }
                        game.state = GAMEOVER;
                        background_init();
                        ui_init();
                        break;
                }
        }
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
        game.end = SDL_GetPerformanceCounter(); 
        float elapsedMS = (game.end - game.start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(33.3333333333f - elapsedMS));
}

void game_set_new_state(int new_state)
{
        game.new_state = new_state;
}

enum State game_get_state(void)
{
        return game.state;
}

int game_get_keyframe(void)
{
        return game.key_frame;
}