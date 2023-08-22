#include "entity.h"
#include "render.h"
#include "input.h"
#include "game.h"

static struct Entity entity;

void entity_init(void)
{
        entity.texture = IMG_LoadTexture(render_get_renderer(), "fish_atlas.png");

        for (int i = 0; i < MAX_FISH; i++) {
                entity.fish_arr[i].active = false;
                entity.fish_arr[i].id = i;
        }

        for (int i = 0; i < LANES; i++) {
                entity.lanes[i].active = false;
                entity.lanes[i].id = i;
        }

        entity.fish_cap           = 16;
        entity.fish_active        = 0;
        entity.fish_spawn_rate    = 20;
        entity.fish_spawn_timer   = 0;
        entity.fish_caught        = 0;
        entity.turtle_spawn_rate  = 30;
        entity.turtle_spawn_timer = 0;
        entity.turtle_active      = false;
        entity.turtles_caught     = 0;
}

void entity_update(void)
{
        if (game_get_state() == PLAY) {
                if (entity.turtles_caught < 3) {
                        entity_spawn_turtle();
                        entity_spawn_fish(); 
                        entity_update_turtle();
                        entity_update_fish();
                }
                if (entity.turtles_caught >= 3) {
                        entity_end_state();
                } 
        }   
}

void entity_end_state(void)
{
        for (int i = 0; i < MAX_FISH; i++) {
                if (entity.fish_arr[i].active) {
                        if (!entity.fish_arr[i].dying) {
                                entity.fish_arr[i].dying = true;
                                entity.fish_arr[i].death_count = 0;
                        }
                        if (entity.fish_arr[i].dying) {
                                entity.fish_arr[i].death_count++;
                                entity.fish_arr[i].src.x = entity.fish_arr[i].src.w * 2;
                                if (entity.fish_arr[i].death_count > 3) {
                                        entity.fish_arr[i].active = false;
                                        entity.fish_active--;
                                }
                        }
                }
        }

        if(entity.fish_active <= 0) {
                game_set_new_state(4);
        }
}

void entity_render(void)
{
        if (game_get_state() == PLAY) {              
                SDL_RenderCopy(render_get_renderer(), entity.texture, &entity.turtle.src, &entity.turtle.dst);
                for (int i = 0; i < MAX_FISH; i++) {
                        if (entity.fish_arr[i].active) {
                                SDL_RenderCopy(render_get_renderer(), entity.texture, &entity.fish_arr[i].src, &entity.fish_arr[i].dst);
                        }
                }
        }
}

void entity_destroy(void)
{
        SDL_DestroyTexture(entity.texture);

        for (int i = 0; i < MAX_FISH; i++) {
                entity.fish_arr[i].active    = false;
                entity.fish_arr[i].id        = 0;
                entity.fish_arr[i].type      = 0;
                entity.fish_arr[i].direction = 0;
                entity.fish_arr[i].speed     = 0;
                entity.fish_arr[i].src       = (SDL_Rect){0, 0, 0, 0};
                entity.fish_arr[i].dst       = (SDL_Rect){0, 0, 0, 0};
        }

        entity.turtle.src = (SDL_Rect){0, 0, 0, 0};
        entity.turtle.dst = (SDL_Rect){0, 0, 0, 0};
}

void entity_spawn_turtle(void)
{
        float scale = render_get_scale();
        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);

        if (entity.turtle_active == false) {

                int spawn_y;
                entity_turtle_randomizer(&spawn_y);

                entity.turtle.direction   = rand() % (2 + 1 - 1) + 1;
                entity.turtle.speed       = rand() % (5 + 1 - 1) + 1;
                entity.turtle.dying       = false;
                entity.turtle.death_count = 0;

                if (entity.turtle.direction == 1) {
                        entity.turtle.src    = (SDL_Rect){0, 256, TURTLE_W, TURTLE_H};
                        entity.turtle.dst    = (SDL_Rect){bounds.x - (TURTLE_W * scale), spawn_y, TURTLE_W * scale, TURTLE_H * scale};
                }
                if (entity.turtle.direction == 2) {
                        entity.turtle.src    = (SDL_Rect){0, 272, TURTLE_W, TURTLE_H};
                        entity.turtle.dst    = (SDL_Rect){bounds.w, spawn_y, TURTLE_W * scale, TURTLE_H * scale};
                }

                entity.turtle_active = true;
        }
}

void entity_update_turtle(void)
{
        if (entity.turtle_active) {

                float scale = render_get_scale();
                SDL_Rect bounds;
                SDL_GetDisplayBounds(0, &bounds);
                bool touch_event = input_get_event();

                if (touch_event) {
                        SDL_Point touch_loc = input_get_touch_loc();
                        SDL_bool collision = SDL_PointInRect(&touch_loc, &entity.turtle.dst);
                        if(collision) {
                                entity.turtle.dying = true;
                                entity.turtle.death_count = 0;
                        }
                }

                if (entity.turtle.dying) {
                        entity.turtle.death_count++;
                        entity.turtle.src.x = entity.turtle.src.w * 2;
                        if (entity.turtle.death_count > 3) {
                                entity.turtle_active = false;
                                entity.turtles_caught++;
                        }
                }

                if (entity.turtle.direction == 1) {
                        entity.turtle.dst.x = entity.turtle.dst.x + entity.turtle.speed;
                        if (entity.turtle.dst.x > bounds.w) {
                                entity.turtle_active = false;  
                        }     
                }

                if (entity.turtle.direction == 2) {
                        entity.turtle.dst.x = entity.turtle.dst.x - entity.turtle.speed;
                        if (entity.turtle.dst.x < bounds.x - (TURTLE_W * scale) ) {
                                entity.turtle_active = false;  
                        }     
                }

                if (!entity.turtle.dying) {
                        if (game_get_keyframe() == 1) {
                                entity.turtle.src.x = 0;
                        }   
                        if (game_get_keyframe() == 2) {
                                entity.turtle.src.x = entity.turtle.src.w;
                        }
                } 
        }
}

void entity_spawn_fish(void)
{      
        entity.fish_spawn_timer++;

        if (entity.fish_active < entity.fish_cap && entity.fish_spawn_timer > entity.fish_spawn_rate) {

                float scale = render_get_scale();
                SDL_Rect bounds;
                SDL_GetDisplayBounds(0, &bounds);

                int open_slot;
                for (int i = 0; i < MAX_FISH; i++) {
                        if (entity.fish_arr[i].active == false) {
                                open_slot = entity.fish_arr[i].id;
                                break;
                        }                      
                }

                int spawn_y;
                int lane;
                entity_fish_randomizer(&spawn_y, &lane);

                entity.fish_arr[open_slot].type        = rand() % (8 + 1 - 1) + 1;
                entity.fish_arr[open_slot].direction   = rand() % (2 + 1 - 1) + 1;
                entity.fish_arr[open_slot].speed       = rand() % (8 + 1 - 3) + 3;
                entity.fish_arr[open_slot].lane        = lane;
                entity.fish_arr[open_slot].active      = true;
                entity.fish_arr[open_slot].dying       = false;
                entity.fish_arr[open_slot].death_count = 0;

                switch (entity.fish_arr[open_slot].type) {           
                case 1:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 0, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 16, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 2:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 32, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 48, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 3:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 64, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 80, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 4:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 96, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (BIGFISH_W * scale), spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 112, BIGFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, BIGFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 5:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 128, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 144, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 6:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 160, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 176, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 7:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 192, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 208, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                case 8:
                        if (entity.fish_arr[open_slot].direction == 1) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 224, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.x - (SMOLFISH_W * scale), spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        } 
                        if (entity.fish_arr[open_slot].direction == 2) {
                                entity.fish_arr[open_slot].src = (SDL_Rect){0, 240, SMOLFISH_W, FISH_H};
                                entity.fish_arr[open_slot].dst = (SDL_Rect){bounds.w, spawn_y, SMOLFISH_W * scale, FISH_H * scale};
                        }
                        break;
                }

                entity.fish_active++;
                entity.fish_spawn_timer = 0;
        }
}

void entity_update_fish(void)
{
        for (int i = 0; i < MAX_FISH; i++) {

                if (entity.fish_arr[i].active == true) {

                        float scale = render_get_scale();
                        SDL_Rect bounds;
                        SDL_GetDisplayBounds(0, &bounds);
                        bool touch_event = input_get_event();

                        if (touch_event) {
                                SDL_Point touch_loc = input_get_touch_loc();
                                SDL_bool collision = SDL_PointInRect(&touch_loc, &entity.fish_arr[i].dst);
                                if(collision) {
                                        entity.fish_arr[i].dying = true;
                                        entity.fish_arr[i].death_count = 0;
                                }
                        }

                        int active_lane = entity.fish_arr[i].lane;

                        if (entity.fish_arr[i].dying) {
                                entity.fish_arr[i].death_count++;
                                entity.fish_arr[i].src.x = entity.fish_arr[i].src.w * 2;
                                if (entity.fish_arr[i].death_count > 3) {
                                        entity.fish_arr[i].active = false;
                                        entity.lanes[active_lane].active = false;
                                        entity.fish_active--;
                                        entity.fish_caught++;
                                }
                        }

                        if (entity.fish_arr[i].direction == 1) {
                                entity.fish_arr[i].dst.x = entity.fish_arr[i].dst.x + entity.fish_arr[i].speed;

                                if (entity.fish_arr[i].dst.x > bounds.w) {
                                        entity.fish_arr[i].active = false;
                                        entity.lanes[active_lane].active = false;
                                        entity.fish_active--;   
                                }     
                        }

                        if (entity.fish_arr[i].direction == 2) {     
                                entity.fish_arr[i].dst.x = entity.fish_arr[i].dst.x - entity.fish_arr[i].speed;

                                if (entity.fish_arr[i].dst.x < bounds.x - (entity.fish_arr[i].src.w * scale) ) {
                                        entity.fish_arr[i].active = false;
                                        entity.lanes[active_lane].active = false;
                                        entity.fish_active--;
                                }
                        }

                        if (!entity.fish_arr[i].dying) {
                                if (game_get_keyframe() == 1) {
                                        if (entity.fish_arr[i].type == 1 || entity.fish_arr[i].type == 3 || entity.fish_arr[i].type == 5 || entity.fish_arr[i].type == 7) {
                                                entity.fish_arr[i].src.x = 0;
                                        }
                                        if (entity.fish_arr[i].type == 2 || entity.fish_arr[i].type == 4 || entity.fish_arr[i].type == 6 || entity.fish_arr[i].type == 8) {
                                                entity.fish_arr[i].src.x = entity.fish_arr[i].src.w;
                                        }                                    
                                }   
                                if (game_get_keyframe() == 2) {
                                        if (entity.fish_arr[i].type == 1 || entity.fish_arr[i].type == 3 || entity.fish_arr[i].type == 5 || entity.fish_arr[i].type == 7) {
                                                entity.fish_arr[i].src.x = entity.fish_arr[i].src.w;
                                        }
                                        if (entity.fish_arr[i].type == 2 || entity.fish_arr[i].type == 4 || entity.fish_arr[i].type == 6 || entity.fish_arr[i].type == 8) {
                                                entity.fish_arr[i].src.x = 0;
                                        }
                                }
                        }     
                }
        }
}

void entity_turtle_randomizer(int *spawn_y)
{
        float scale = render_get_scale();

        int assigned_lane = rand() % (14 + 1 - 1) + 1;

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

void entity_fish_randomizer(int *spawn_y, int *lane)
{
        float scale = render_get_scale();
        int assigned_lane;

        for (int i = 1; i < LANES; i++) {
                if (entity.lanes[i].active == false) {
                        assigned_lane = entity.lanes[i].id;
                        entity.lanes[i].active = true;
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

int entity_get_fish_caught(void)
{
        return entity.fish_caught;
}

int entity_get_turtles_caught(void)
{
        return entity.turtles_caught;
}