#pragma once

#include "common.h"

enum State {
        PLAY,
        QUIT
};

struct Game {
        bool running;
        int frame_count;
        int key_frame;
        int start;
        int end;
        enum State state;
};

void game_loop(void);
void game_init(void);
void game_update(void);
void game_render(void);
void game_frame_count(void);
void game_fps_cap(void);
int game_get_keyframe(void);
void game_change_state(int new_state);



