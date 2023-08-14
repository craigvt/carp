#include "entity.h"
#include "render.h"
#include "game.h"
#include "input.h"

static entity_context ctx;

entity_context *entity_get_context(void)
{
        return &ctx;
}

void entity_init_play(void)
{
        render_context *render_ctx = render_get_context();

        ctx.texture = IMG_LoadTexture(render_ctx->render, "fish_atlas.png");

        uint8_t fish_count = 0;
        for (uint8_t i = 0; i < MAX_FISH; i++) {
                ctx.fish_arr[i].active = false;
                ctx.fish_arr[i].id = fish_count;
                fish_count++;
        }

        uint8_t lane_count = 0;
        for (uint8_t i = 0; i < LANES; i++) {
                ctx.lanes[i].active = false;
                ctx.lanes[i].id = lane_count;
                lane_count++;
        }

        ctx.fish_cap           = 10;
        ctx.fish_active        = 0;
        ctx.fish_spawn_rate    = 30;
        ctx.fish_spawn_timer   = 0;
        ctx.turtle_spawn_rate  = 30;
        ctx.turtle_spawn_timer = 0;
        ctx.turtle_active      = false;
        ctx.turtles_caught     = 0;
}

void entity_update_play(void)
{
        entity_spawn_fish(); 
        entity_spawn_turtle();
        entity_update_fish();
        entity_update_turtle();
}

void entity_render_play(void)
{
        render_context *render_ctx = render_get_context();

        for (uint8_t i = 0; i < MAX_FISH; i++) {
                if (ctx.fish_arr[i].active == true) {
                        SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.fish_arr[i].src, &ctx.fish_arr[i].dst);
                }
        }

        if (ctx.turtle_active) {
                SDL_RenderCopy(render_ctx->render, ctx.texture, &ctx.turtle.src, &ctx.turtle.dst);
        }
}

void entity_destroy_play(void)
{
        SDL_DestroyTexture(ctx.texture);

        for (uint8_t i = 0; i < MAX_FISH; i++) {
                ctx.fish_arr[i].active    = false;
                ctx.fish_arr[i].id        = 0;
                ctx.fish_arr[i].type      = 0;
                ctx.fish_arr[i].direction = 0;
                ctx.fish_arr[i].speed     = 0;
                ctx.fish_arr[i].src       = (SDL_Rect){0, 0, 0, 0};
                ctx.fish_arr[i].dst       = (SDL_Rect){0, 0, 0, 0};
        }

        ctx.turtle.src = (SDL_Rect){0, 0, 0, 0};
        ctx.turtle.dst = (SDL_Rect){0, 0, 0, 0};
}

void entity_spawn_turtle(void)
{
        if (!ctx.turtle_active) {

                ctx.turtle.direction   = rand() % 2;
                ctx.turtle.speed       = rand() % (2 + 1 - 1) + 1;
                ctx.turtle.dying       = false;
                ctx.turtle.death_count = 0;

                int spawn_y;
                uint8_t lane;
                entity_spawn_randomizer(&spawn_y, &lane);

                if (ctx.turtle.direction == 0) {
                        ctx.turtle.src    = (SDL_Rect){0, 192, TURTLE_W, TURTLE_H};
                        ctx.turtle.dst    = (SDL_Rect){0 - TURTLE_W, spawn_y, TURTLE_W, TURTLE_H};
                        ctx.turtle.lane   = lane;
                        ctx.turtle_active = true;
                }
                if (ctx.turtle.direction == 1) {
                        ctx.turtle.src = (SDL_Rect){0, 208, TURTLE_W, TURTLE_H};
                        ctx.turtle.dst = (SDL_Rect){PLAYBG_W, spawn_y, TURTLE_W, TURTLE_H};
                        ctx.turtle.lane   = lane;
                        ctx.turtle_active = true;
                }
        }
}

void entity_update_turtle(void)
{
        if (ctx.turtle_active) {

                input_context *input_ctx = input_get_context();
                game_context *game_ctx   = game_get_context();

                uint8_t active_lane = ctx.turtle.lane;

                if (ctx.turtle.dying) {
                        ctx.turtle.death_count++;
                        if (ctx.turtle.death_count > 3) {
                                ctx.turtle_active = false;
                                ctx.lanes[active_lane].active = false;
                        }
                }

                if (ctx.turtle.direction == 0) {
                        ctx.turtle.dst.x = ctx.turtle.dst.x + ctx.turtle.speed;

                        if (ctx.turtle.dst.x > PLAYBG_W) {
                                ctx.lanes[active_lane].active = false;
                                ctx.turtle_active = false;  
                        }     
                }

                if (ctx.turtle.direction == 1) {
                        ctx.turtle.dst.x = ctx.turtle.dst.x - ctx.turtle.speed;

                        if (ctx.turtle.dst.x < 0 - TURTLE_W) {
                                ctx.lanes[active_lane].active = false;
                                ctx.turtle_active = false;  
                        }     
                }

                if (!ctx.turtle.dying) {
                        if (game_ctx->key_frame == 1) {
                                ctx.turtle.src.x = 0;
                        }   
                        if (game_ctx->key_frame == 2) {
                                ctx.turtle.src.x = ctx.turtle.src.w;
                        }
                } 

                if (ctx.turtle.dying) {
                        ctx.turtle.src.x = ctx.turtle.src.w * 2;
                }
        }
}

void entity_spawn_fish(void)
{      
        ctx.fish_spawn_timer++;

        if (ctx.fish_active < ctx.fish_cap && ctx.fish_spawn_timer > ctx.fish_spawn_rate) {

                uint8_t open_slot;
                for (uint8_t i = 0; i < MAX_FISH; i++) {
                        if (ctx.fish_arr[i].active == false) {
                                open_slot = ctx.fish_arr[i].id;
                                break;
                        }                      
                }

                int spawn_y;
                uint8_t lane;
                entity_spawn_randomizer(&spawn_y, &lane);

                ctx.fish_arr[open_slot].type        = rand() % (8 + 1 - 1) + 1;
                ctx.fish_arr[open_slot].direction   = rand() % 2;
                ctx.fish_arr[open_slot].speed       = rand() % (2 + 1 - 1) + 1;
                ctx.fish_arr[open_slot].lane        = lane;
                ctx.fish_arr[open_slot].active      = true;
                ctx.fish_arr[open_slot].dying       = false;
                ctx.fish_arr[open_slot].death_count = 0;

                switch (ctx.fish_arr[open_slot].type) {           
                case 1:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 0, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - BIGFISH_W, spawn_y, BIGFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 16, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, BIGFISH_W, FISH_H};
                        }
                        break;
                case 2:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 32, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - BIGFISH_W, spawn_y, BIGFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 48, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, BIGFISH_W, FISH_H};
                        }
                        break;
                case 3:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 64, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - BIGFISH_W, spawn_y, BIGFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 80, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, BIGFISH_W, FISH_H};
                        }
                        break;
                case 4:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 96, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - BIGFISH_W, spawn_y, BIGFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 112, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, BIGFISH_W, FISH_H};
                        }
                        break;
                case 5:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 128, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - SMOLFISH_W, spawn_y, SMOLFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 144, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, SMOLFISH_W, FISH_H};
                        }
                        break;
                case 6:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 160, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - SMOLFISH_W, spawn_y, SMOLFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 176, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, SMOLFISH_W, FISH_H};
                        }
                        break;
                case 7:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){48, 128, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - SMOLFISH_W, spawn_y, SMOLFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){48, 144, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, SMOLFISH_W, FISH_H};
                        }
                        break;
                case 8:
                        if (ctx.fish_arr[open_slot].direction == 0) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){48, 160, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){0 - SMOLFISH_W, spawn_y, SMOLFISH_W, FISH_H};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){48, 176, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){PLAYBG_W, spawn_y, SMOLFISH_W, FISH_H};
                        }
                        break;
                }

                ctx.fish_active++;
                ctx.fish_spawn_timer = 0;
        }
}

void entity_update_fish(void)
{
        input_context *input_ctx = input_get_context();
        game_context *game_ctx   = game_get_context();

        for (uint8_t i = 0; i < MAX_FISH; i++) {

                if (ctx.fish_arr[i].active == true) {

                        uint8_t active_lane = ctx.fish_arr[i].lane;

                        if (ctx.fish_arr[i].dying) {
                                ctx.fish_arr[i].death_count++;
                                if (ctx.fish_arr[i].death_count > 3) {
                                        ctx.fish_arr[i].active = false;
                                        ctx.lanes[active_lane].active = false;
                                        ctx.fish_active--;
                                }
                        }

                        if (ctx.fish_arr[i].direction == 0) {
                                ctx.fish_arr[i].dst.x = ctx.fish_arr[i].dst.x + ctx.fish_arr[i].speed;

                                if (ctx.fish_arr[i].dst.x > PLAYBG_W) {
                                        ctx.fish_arr[i].active = false;
                                        ctx.lanes[active_lane].active = false;
                                        ctx.fish_active--;   
                                }     
                        }

                        if (ctx.fish_arr[i].direction == 1) {     
                                ctx.fish_arr[i].dst.x = ctx.fish_arr[i].dst.x - ctx.fish_arr[i].speed;

                                if (ctx.fish_arr[i].dst.x < 0 - ctx.fish_arr[i].src.w) {
                                        ctx.fish_arr[i].active = false;
                                        ctx.lanes[active_lane].active = false;
                                        ctx.fish_active--;
                                }
                        }

                        if (!ctx.fish_arr[i].dying) {
                                if (game_ctx->key_frame == 1) {
                                        if (ctx.fish_arr[i].type == 1 || ctx.fish_arr[i].type == 3 || ctx.fish_arr[i].type == 5 || ctx.fish_arr[i].type == 7) {
                                                ctx.fish_arr[i].src.x = 0;
                                        }
                                        if (ctx.fish_arr[i].type == 2 || ctx.fish_arr[i].type == 4 || ctx.fish_arr[i].type == 6 || ctx.fish_arr[i].type == 8) {
                                                ctx.fish_arr[i].src.x = ctx.fish_arr[i].src.w;
                                        }                                    
                                }   
                                if (game_ctx->key_frame == 2) {
                                        if (ctx.fish_arr[i].type == 1 || ctx.fish_arr[i].type == 3 || ctx.fish_arr[i].type == 5 || ctx.fish_arr[i].type == 7) {
                                                ctx.fish_arr[i].src.x = ctx.fish_arr[i].src.w;
                                        }
                                        if (ctx.fish_arr[i].type == 2 || ctx.fish_arr[i].type == 4 || ctx.fish_arr[i].type == 6 || ctx.fish_arr[i].type == 8) {
                                                ctx.fish_arr[i].src.x = 0;
                                        }
                                }
                        }   

                        if (ctx.fish_arr[i].dying) {
                                ctx.fish_arr[i].src.x = ctx.fish_arr[i].src.w * 2;
                        }    
                }
        }
}

void entity_spawn_randomizer(int *spawn_y, uint8_t *lane)
{
        uint8_t assigned_lane;

        for (uint8_t i = 0; i < LANES; i++) {
                if (ctx.lanes[i].active == false) {
                        assigned_lane = ctx.lanes[i].id;
                        ctx.lanes[i].active = true;
                        break;
                }
        }

        switch (assigned_lane) {
        case 0:  
                *spawn_y = 48;
                *lane = 0; 
                break;
        case 1:  
                *spawn_y = 64;
                *lane = 1; 
                break;
        case 2:  
                *spawn_y = 80;
                *lane = 2;
                break;
        case 3:  
                *spawn_y = 96;
                *lane = 3;
                break;
        case 4:  
                *spawn_y = 112;
                *lane = 4;
                break;
        case 5:  
                *spawn_y = 128;
                *lane = 5;
                break;
        case 6:  
                *spawn_y = 144; 
                *lane = 6;
                break;
        case 7:  
                *spawn_y = 160;
                *lane = 7;
                break;
        case 8:  
                *spawn_y = 176;
                *lane = 8;
                break;
        case 9: 
                *spawn_y = 192;
                *lane = 9;
                break;
        case 10: 
                *spawn_y = 208;
                *lane = 10;
                break;
        case 11: 
                *spawn_y = 224;
                *lane = 11; 
                break;
        case 12: 
                *spawn_y = 240;
                *lane = 12; 
                break;
        case 13: 
                *spawn_y = 256;
                *lane = 13;
                break;
        }
}