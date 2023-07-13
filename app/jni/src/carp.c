/*
 * Copyright (c) 2023, Craig Webb.  All rights reserved.
 * Author: Craig Webb <craigvt@comcast.net>
 */

#include "carp.h"

int main(int argc, char* args[])
{	   
        srand(time(NULL));

        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);

        SDL_Window *window = init_window();
        SDL_Renderer *renderer = init_renderer(window);

        bool running    = true;
        int fish_active = 0;
        int fish_cap    = 3;
        int frame_count = 0;
        int key_frame   = 1;
        SDL_Event e;

        SDL_Texture *bg_tex   = create_texture(renderer, "play_bg.png", 1, 255);
        SDL_Texture *fish_tex = create_texture(renderer, "fish_atlas.png", 1, 255);

        Object bg = create_object(0, 0, get_offset_width(), 0, BG_W, BG_H);

        Fish_arr fish = init_fish();

        while (running) {

                while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT) {
                                running = false;
                        }              
                }

                fish_spawn(&fish_active, &key_frame, &fish_cap, &fish);
                fish_update(&fish_active, &key_frame, &fish);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, bg_tex, &bg.src_rect, &bg.dst_rect);
                fish_render(renderer, fish_tex, &fish);
                SDL_RenderPresent(renderer);  

                frame_count++; 
                if (frame_count >= 15) {
                        if (key_frame < 5) {
                                key_frame++;
                                frame_count = 0;
                        }
                        if (key_frame == 5) {
                                key_frame = 1;
                                frame_count = 0;
                        }
                }
        }

        SDL_DestroyTexture(bg_tex);
        SDL_DestroyTexture(fish_tex);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 0;
}

SDL_Window *init_window(void)
{
        SDL_Window *window = SDL_CreateWindow("Carp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, NATIVE_W, NATIVE_H, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL); 
        
        return window; 
}

SDL_Renderer *init_renderer(SDL_Window *window)
{
        float    scale_factor;
        int      scaled_width;
        SDL_Rect bounds;

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        SDL_GetDisplayBounds(0, &bounds);
        scale_factor = (float)bounds.h / NATIVE_H;

        SDL_RenderSetScale(renderer, scale_factor, scale_factor);

        return renderer;
}

int get_offset_width(void)
{
        float    scale_factor;
        int      scaled_width, offset;
        SDL_Rect bounds;

        SDL_GetDisplayBounds(0, &bounds);
        scale_factor = (float)bounds.h / NATIVE_H;

        scaled_width = NATIVE_W * scale_factor;
        offset = (scaled_width - bounds.w) / 2;

        return offset;
}

SDL_Texture *create_texture(SDL_Renderer *renderer, const char *str, SDL_BlendMode blendMode, Uint8 alpha)
{
        SDL_Texture *texture = IMG_LoadTexture(renderer, str);
        SDL_SetTextureBlendMode(texture, blendMode);
        SDL_SetTextureAlphaMod(texture, alpha);

        return texture;
}

Object create_object(int src_x, int src_y, int dst_x, int dst_y, int w, int h)
{
        Object object;

        object.src_rect = (SDL_Rect){src_x, src_y, w, h};
        object.dst_rect = (SDL_Rect){dst_x, dst_y, w, h};

        return object;
}

Fish_arr init_fish(void)
{
        Fish_arr fish;

        int count = 0;
        for (int i = 0; i < MAX_FISH; i++) {
                fish.fish_arr[i].id = count;
                fish.fish_arr[i].active = false;
                count++;
        }

        return fish;
}

void fish_spawn(int *fish_active, int *key_frame, int *fish_cap, Fish_arr *fish)
{
        if (*fish_active < *fish_cap) {

                int open_slot;
                for (int i = 0; i < MAX_FISH; i++) {
                        if (fish->fish_arr[i].active == false) {
                                open_slot = fish->fish_arr[i].id;
                                break;
                        }                      
                }

                int fish_type = 1;
                int fish_dir  = rand() % 2;
                int fish_speed = rand() % (10 + 1 - 3) + 3;
                int fish_y_axis = rand() % (2000 + 1 - 50) + 50;
                int src_x, src_y, src_w, src_h;
                int dst_x, dst_y;

                switch (fish_type) {          
                case 0:
                        if (fish_dir == 0) {
                                src_x = 0;
                                src_y = 0;
                                src_w = 640;
                                src_h = 256;
                                dst_x = -src_w;
                                dst_y = fish_y_axis;  
                        } 
                        if (fish_dir == 1) {
                                src_x = 0;
                                src_y = 256;
                                src_w = 640;
                                src_h = 256;
                                dst_x = BG_W + src_w;
                                dst_y = fish_y_axis;
                        }
                        break;  
                case 1:
                        if (fish_dir == 0) {
                                src_x = 0;
                                src_y = 0;
                                src_w = 640;
                                src_h = 256;
                                dst_x = -src_w;
                                dst_y = fish_y_axis;  
                        } 
                        if (fish_dir == 1) {
                                src_x = 0;
                                src_y = 256;
                                src_w = 640;
                                src_h = 256;
                                dst_x = BG_W + src_w;
                                dst_y = fish_y_axis;
                        }
                        break;  
                }

                fish->fish_arr[open_slot].type       = fish_type;
                fish->fish_arr[open_slot].dir        = fish_dir;
                fish->fish_arr[open_slot].speed      = fish_speed;
                fish->fish_arr[open_slot].src        = (SDL_Rect){src_x, src_y, src_w, src_h};
                fish->fish_arr[open_slot].dst        = (SDL_Rect){dst_x, dst_y, src_w, src_h};
                fish->fish_arr[open_slot].active     = true;
                fish->fish_arr[open_slot].key_frame  = *key_frame;

                (*fish_active)++;
        }
}

void fish_render(SDL_Renderer *renderer, SDL_Texture *fish_tex, Fish_arr *fish)
{
        for (int i = 0; i < MAX_FISH; i++) {
                if (fish->fish_arr[i].active == true) {
                        SDL_RenderCopy(renderer, fish_tex, &fish->fish_arr[i].src, &fish->fish_arr[i].dst);
                }
        }
}

void fish_update(int *fish_active, int *key_frame, Fish_arr *fish)
{
        for (int i = 0; i < MAX_FISH; i++) {
                if (fish->fish_arr[i].active == true) {
                        if (*key_frame > fish->fish_arr[i].key_frame | *key_frame < fish->fish_arr[i].key_frame) {
                                if (*key_frame < 5) {
                                        fish->fish_arr[i].src.x = fish->fish_arr[i].src.x + fish->fish_arr[i].src.w;
                                        fish->fish_arr[i].key_frame = *key_frame;
                                }
                                if (*key_frame == 1) {
                                        fish->fish_arr[i].src.x = 0;
                                        fish->fish_arr[i].key_frame = *key_frame;
                                }                              
                        }
                        if (fish->fish_arr[i].dir == 0) {
                                fish->fish_arr[i].dst.x = fish->fish_arr[i].dst.x + fish->fish_arr[i].speed;
                                if (fish->fish_arr[i].dst.x > BG_W) {
                                        fish->fish_arr[i].active = false;
                                        (*fish_active)--;    
                                }
                                
                        }
                        if (fish->fish_arr[i].dir == 1) {     
                                fish->fish_arr[i].dst.x = fish->fish_arr[i].dst.x - fish->fish_arr[i].speed;
                                if (fish->fish_arr[i].dst.x < 0 - fish->fish_arr[i].src.w) {
                                        fish->fish_arr[i].active = false;
                                        (*fish_active)--;
                                }
                        }             
                }
        }
}