#ifndef carp_h
#define carp_h

#include "SDL.h"
#include "SDL_image.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <android/log.h>

#define  LOG_TAG    "carp"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define NATIVE_W 1440
#define NATIVE_H 2560
#define BG_W     1640
#define BG_H     2560
#define MAX_FISH 20

typedef struct {
        SDL_Texture *tex;
} Texture;

typedef struct {
        SDL_Rect src_rect;
        SDL_Rect dst_rect;
} Object;

typedef struct {
        bool active;
        uint8_t  id;
        uint8_t  type;
        uint8_t  dir;
        uint8_t  speed;
        uint8_t  key_frame;
        SDL_Rect src;
        SDL_Rect dst;
} Fish;

typedef struct {
        Fish fish_arr[MAX_FISH];
} Fish_arr;

typedef struct {
        bool touched;
        SDL_Event event;
        SDL_Point touch_loc;
} Input;

SDL_Window *init_window(void);
SDL_Renderer *init_renderer(SDL_Window *window);
int get_offset_width(void);

Input *get_input(bool *running, Input *input);

SDL_Texture *create_texture(SDL_Renderer *renderer, const char *str, SDL_BlendMode blendMode, Uint8 alpha);
Object create_object(int src_x, int src_y, int dst_x, int dst_y, int w, int h);

Fish_arr init_fish(void);
void fish_spawn(uint8_t *fish_active, uint8_t *key_frame, uint8_t *fish_cap, Fish_arr *fish);
void fish_render(SDL_Renderer *renderer, SDL_Texture *fish_tex, Fish_arr *fish);
void fish_update(uint8_t *fish_active, uint8_t *key_frame, Fish_arr *fish, Input *input);

#endif
