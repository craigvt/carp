#include "entity.h"
#include "render.h"
#include "game.h"
#include "input.h"

static entity_context ctx;

entity_context *entity_get_context(void)
{
        return &ctx;
}

void entity_init_fish(void)
{
        render_context *render_ctx = render_get_context();

        ctx.fish_texture = IMG_LoadTexture(render_ctx->render, "fish_atlas.png");

        uint8_t count = 0;
        for (uint8_t i = 0; i < MAX_FISH; i++) {
                ctx.fish_arr[i].active = false;
                ctx.fish_arr[i].id = count;
                count++;
        }

        ctx.fish_cap    = 3;
        ctx.fish_active = 0;
        ctx.fish_caught = 0;
}

void entity_spawn_fish(void)
{      
        if (ctx.fish_active < ctx.fish_cap) {

                game_context *game_ctx = game_get_context();

                uint8_t open_slot;
                for (uint8_t i = 0; i < MAX_FISH; i++) {
                        if (ctx.fish_arr[i].active == false) {
                                open_slot = ctx.fish_arr[i].id;
                                break;
                        }                      
                }

                uint8_t  fish_type   = 1;
                uint8_t  fish_dir    = rand() % 2;
                uint8_t  fish_speed  = rand() % (10 + 1 - 3) + 3;
                uint16_t fish_y_axis = rand() % (2000 + 1 - 50) + 50;
                int src_x, src_y, src_w, src_h;
                int dst_x, dst_y;

                switch (fish_type) {           
                case 1:
                        if (fish_dir == 0) {
                                src_x = 0;
                                src_y = 0;
                                src_w = 420;
                                src_h = 150;
                                dst_x = -src_w;
                                dst_y = fish_y_axis;  
                        } 
                        if (fish_dir == 1) {
                                src_x = 0;
                                src_y = 150;
                                src_w = 420;
                                src_h = 150;
                                dst_x = PLAYBG_W + src_w;
                                dst_y = fish_y_axis;
                        }
                        break;  
                }

                ctx.fish_arr[open_slot].type       = fish_type;
                ctx.fish_arr[open_slot].dir        = fish_dir;
                ctx.fish_arr[open_slot].speed      = fish_speed;
                ctx.fish_arr[open_slot].src        = (SDL_Rect){src_x, src_y, src_w, src_h};
                ctx.fish_arr[open_slot].dst        = (SDL_Rect){dst_x, dst_y, src_w, src_h};
                ctx.fish_arr[open_slot].active     = true;
                ctx.fish_arr[open_slot].key_frame  = game_ctx->key_frame;

                ctx.fish_active++;
        }
}

void entity_update_fish(void)
{
        input_context *input_ctx  = input_get_context();
        game_context  *game_ctx   = game_get_context();

        for (uint8_t i = 0; i < MAX_FISH; i++) {

                if (ctx.fish_arr[i].active == true) {

                        // check for touched fish
                        if (input_ctx->action_id == TOUCH) {
                                if ((input_ctx->touch_loc.x > ctx.fish_arr[i].dst.x) && (input_ctx->touch_loc.x < ctx.fish_arr[i].dst.x + ctx.fish_arr[i].dst.w)
                                        && (input_ctx->touch_loc.y > ctx.fish_arr[i].dst.y) && (input_ctx->touch_loc.y < ctx.fish_arr[i].dst.y + ctx.fish_arr[i].dst.h)) {
                                                ctx.fish_arr[i].active = false;
                                                ctx.fish_active--;
                                }
                        }

                        // update keyframe
                        if (game_ctx->key_frame > ctx.fish_arr[i].key_frame | game_ctx->key_frame < ctx.fish_arr[i].key_frame) {
                                if (game_ctx->key_frame < 5) {
                                        ctx.fish_arr[i].src.x = ctx.fish_arr[i].src.x + ctx.fish_arr[i].src.w;
                                        ctx.fish_arr[i].key_frame = game_ctx->key_frame;
                                }
                                if (game_ctx->key_frame == 1) {
                                        ctx.fish_arr[i].src.x = 0;
                                        ctx.fish_arr[i].key_frame = game_ctx->key_frame;
                                }                              
                        }

                        // update fish if moving dir 0
                        if (ctx.fish_arr[i].dir == 0) {
                                ctx.fish_arr[i].dst.x = ctx.fish_arr[i].dst.x + ctx.fish_arr[i].speed;

                                // destroy if off screen
                                if (ctx.fish_arr[i].dst.x > PLAYBG_W) {
                                        ctx.fish_arr[i].active = false;
                                        ctx.fish_active--;   
                                }                              
                        }
                        
                        // update fish if moving dir 1
                        if (ctx.fish_arr[i].dir == 1) {     
                                ctx.fish_arr[i].dst.x = ctx.fish_arr[i].dst.x - ctx.fish_arr[i].speed;

                                // destroy if off screen
                                if (ctx.fish_arr[i].dst.x < 0 - ctx.fish_arr[i].src.w) {
                                        ctx.fish_arr[i].active = false;
                                        ctx.fish_active--;
                                }
                        }             
                }
        }
}

void entity_destroy_fish(void)
{
        SDL_DestroyTexture(ctx.fish_texture);

        for (uint8_t i = 0; i < MAX_FISH; i++) {
                ctx.fish_arr[i].active    = false;
                ctx.fish_arr[i].id        = 0;
                ctx.fish_arr[i].type      = 0;
                ctx.fish_arr[i].dir       = 0;
                ctx.fish_arr[i].speed     = 0;
                ctx.fish_arr[i].key_frame = 0;
                ctx.fish_arr[i].src       = (SDL_Rect){0, 0, 0, 0};
                ctx.fish_arr[i].dst       = (SDL_Rect){0, 0, 0, 0};
        }
}

void entity_render_play(void)
{
        render_context *render_ctx = render_get_context();

        for (uint8_t i = 0; i < MAX_FISH; i++) {
                if (ctx.fish_arr[i].active == true) {
                        SDL_RenderCopy(render_ctx->render, ctx.fish_texture, &ctx.fish_arr[i].src, &ctx.fish_arr[i].dst);
                }
        }
}