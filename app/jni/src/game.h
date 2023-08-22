#pragma once

#include "common.h"

enum State {
        QUIT,
        LOGO,
        TITLE,
        PLAY,
        GAMEOVER
};

struct Game {
        bool running;
        int frame_count;
        int key_frame;
        int start;
        int end;
        int new_state;
        enum State state;
};

void game_loop(void);
void game_init(void);
void game_state_manager(void);
void game_frame_count(void);
void game_fps_cap(void);
int game_get_keyframe(void);
void game_set_new_state(int new_state);
enum State game_get_state(void);



