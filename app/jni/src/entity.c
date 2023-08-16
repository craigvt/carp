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

        ctx.fish_cap           = 16;
        ctx.fish_active        = 0;
        ctx.fish_spawn_rate    = 20;
        ctx.fish_spawn_timer   = 0;
        ctx.fish_caught        = 0;
        ctx.turtle_spawn_rate  = 30;
        ctx.turtle_spawn_timer = 0;
        ctx.turtle_active      = false;
        ctx.turtles_caught     = 0;
}

void entity_update_play(void)
{
        entity_spawn_turtle();
        entity_spawn_fish(); 
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
        render_context *render_ctx = render_get_context();
        float scale = render_ctx->scale_factor;

        if (ctx.turtle_active == false) {

                int spawn_y;
                entity_turtle_randomizer(&spawn_y);

                ctx.turtle.direction   = rand() % (2 + 1 - 1) + 1;
                ctx.turtle.speed       = rand() % (5 + 1 - 1) + 1;
                ctx.turtle.dying       = false;
                ctx.turtle.death_count = 0;

                if (ctx.turtle.direction == 1) {
                        ctx.turtle.src    = (SDL_Rect){0, 256, TURTLE_W, TURTLE_H};
                        ctx.turtle.dst    = (SDL_Rect){render_ctx->bounds.x - (TURTLE_W * scale), spawn_y, TURTLE_W * scale, TURTLE_H * scale};
                }
                if (ctx.turtle.direction == 2) {
                        ctx.turtle.src    = (SDL_Rect){0, 272, TURTLE_W, TURTLE_H};
                        ctx.turtle.dst    = (SDL_Rect){render_ctx->bounds.w, spawn_y, TURTLE_W * scale, TURTLE_H * scale};
                }

                ctx.turtle_active = true;
        }
}

void entity_update_turtle(void)
{
        render_context *render_ctx = render_get_context();
        float scale = render_ctx->scale_factor;

        if (ctx.turtle_active) {

                input_context *input_ctx = input_get_context();
                game_context *game_ctx   = game_get_context();

                if (ctx.turtle.dying) {
                        ctx.turtle.death_count++;
                        if (ctx.turtle.death_count > 3) {
                                ctx.turtle_active = false;
                                ctx.turtles_caught++;
                        }
                }

                if (ctx.turtle.direction == 1) {
                        ctx.turtle.dst.x = ctx.turtle.dst.x + ctx.turtle.speed;
                        if (ctx.turtle.dst.x > render_ctx->bounds.w) {
                                ctx.turtle_active = false;  
                        }     
                }

                if (ctx.turtle.direction == 2) {
                        ctx.turtle.dst.x = ctx.turtle.dst.x - ctx.turtle.speed;
                        if (ctx.turtle.dst.x < render_ctx->bounds.x - (TURTLE_W * scale) ) {
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

                render_context *render_ctx = render_get_context();
                float scale = render_ctx->scale_factor;

                uint8_t open_slot;
                for (uint8_t i = 0; i < MAX_FISH; i++) {
                        if (ctx.fish_arr[i].active == false) {
                                open_slot = ctx.fish_arr[i].id;
                                break;
                        }                      
                }

                int spawn_y;
                uint8_t lane;
                entity_fish_randomizer(&spawn_y, &lane);

                ctx.fish_arr[open_slot].type        = rand() % (8 + 1 - 1) + 1;
                ctx.fish_arr[open_slot].direction   = rand() % (2 + 1 - 1) + 1;
                ctx.fish_arr[open_slot].speed       = rand() % (8 + 1 - 3) + 3;
                ctx.fish_arr[open_slot].lane        = lane;
                ctx.fish_arr[open_slot].active      = true;
                ctx.fish_arr[open_slot].dying       = false;
                ctx.fish_arr[open_slot].death_count = 0;

                switch (ctx.fish_arr[open_slot].type) {           
                case 1:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 0, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 16, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 2:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 32, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 48, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 3:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 64, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 80, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 4:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 96, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 112, BIGFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 5:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 128, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 144, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 6:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 160, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 176, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 7:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 192, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 208, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 8:
                        if (ctx.fish_arr[open_slot].direction == 1) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 224, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (ctx.fish_arr[open_slot].direction == 2) {
                                ctx.fish_arr[open_slot].src = (SDL_Rect){0, 240, SMOLFISH_W, FISH_H};
                                ctx.fish_arr[open_slot].dst = (SDL_Rect){render_ctx->bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                }

                ctx.fish_active++;
                ctx.fish_spawn_timer = 0;
        }
}

void entity_update_fish(void)
{
        input_context *input_ctx   = input_get_context();
        game_context *game_ctx     = game_get_context();
        render_context *render_ctx = render_get_context();

        float scale = render_ctx->scale_factor;

        for (uint8_t i = 0; i < MAX_FISH; i++) {

                if (ctx.fish_arr[i].active == true) {

                        uint8_t active_lane = ctx.fish_arr[i].lane;

                        if (ctx.fish_arr[i].dying) {
                                ctx.fish_arr[i].death_count++;
                                if (ctx.fish_arr[i].death_count > 3) {
                                        ctx.fish_arr[i].active = false;
                                        ctx.lanes[active_lane].active = false;
                                        ctx.fish_caught++;
                                        ctx.fish_active--;
                                }
                        }

                        if (ctx.fish_arr[i].direction == 1) {
                                ctx.fish_arr[i].dst.x = ctx.fish_arr[i].dst.x + ctx.fish_arr[i].speed;

                                if (ctx.fish_arr[i].dst.x > render_ctx->bounds.w) {
                                        ctx.fish_arr[i].active = false;
                                        ctx.lanes[active_lane].active = false;
                                        ctx.fish_active--;   
                                }     
                        }

                        if (ctx.fish_arr[i].direction == 2) {     
                                ctx.fish_arr[i].dst.x = ctx.fish_arr[i].dst.x - ctx.fish_arr[i].speed;

                                if (ctx.fish_arr[i].dst.x < render_ctx->bounds.x - (ctx.fish_arr[i].src.w * scale) ) {
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

void entity_turtle_randomizer(int *spawn_y)
{
        render_context *render_ctx = render_get_context();
        float scale = render_ctx->scale_factor;

        uint8_t assigned_lane = rand() % (14 + 1 - 1) + 1;

        switch (assigned_lane) {
                case 1:  *spawn_y = 48  * scale; break;
                case 2:  *spawn_y = 64  * scale; break;
                case 3:  *spawn_y = 80  * scale; break;
                case 4:  *spawn_y = 96  * scale; break;
                case 5:  *spawn_y = 112 * scale; break;
                case 6:  *spawn_y = 128 * scale; break;
                case 7:  *spawn_y = 144 * scale; break;
                case 8:  *spawn_y = 160 * scale; break;
                case 9:  *spawn_y = 176 * scale; break;
                case 10: *spawn_y = 192 * scale; break;
                case 11: *spawn_y = 208 * scale; break;
                case 12: *spawn_y = 224 * scale; break;
                case 13: *spawn_y = 240 * scale; break;
                case 14: *spawn_y = 256 * scale; break;
        }     
}

void entity_fish_randomizer(int *spawn_y, uint8_t *lane)
{
        render_context *render_ctx = render_get_context();
        float scale = render_ctx->scale_factor;
        uint8_t assigned_lane;

        for (uint8_t i = 1; i < LANES; i++) {
                if (ctx.lanes[i].active == false) {
                        assigned_lane = ctx.lanes[i].id;
                        ctx.lanes[i].active = true;
                        break;
                }
        }

        switch (assigned_lane) {
                case 1:  *spawn_y = 48  * scale; *lane = 1;  break;
                case 2:  *spawn_y = 64  * scale; *lane = 2;  break;
                case 3:  *spawn_y = 80  * scale; *lane = 3;  break;
                case 4:  *spawn_y = 96  * scale; *lane = 4;  break;
                case 5:  *spawn_y = 112 * scale; *lane = 5;  break;
                case 6:  *spawn_y = 128 * scale; *lane = 6;  break;
                case 7:  *spawn_y = 144 * scale; *lane = 7;  break;
                case 8:  *spawn_y = 160 * scale; *lane = 8;  break;
                case 9:  *spawn_y = 176 * scale; *lane = 9;  break;
                case 10: *spawn_y = 192 * scale; *lane = 10; break;
                case 11: *spawn_y = 208 * scale; *lane = 11; break;
                case 12: *spawn_y = 224 * scale; *lane = 12; break;
                case 13: *spawn_y = 240 * scale; *lane = 13; break;
                case 14: *spawn_y = 256 * scale; *lane = 14; break;
        }
}