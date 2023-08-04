#pragma once

#include "common.h"

typedef enum {
        PLAY,
        QUIT
} game_state;

typedef struct {
        bool running;
        game_state current_state;
        game_state new_state;
        uint8_t    frame_count;
        uint8_t    key_frame;
} game_context;

game_context *game_get_context(void);
void game(void);
void game_init(void);
void game_state_manager(void);
void game_update(void);
void game_render(void);
void game_frame_count(void);