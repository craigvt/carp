#pragma once

#include "common.h"

struct Input {
        SDL_Event event;
        SDL_Point touch_loc;
        bool touch_event;
};

void input_poll_events(void);
bool input_get_event(void);
SDL_Point input_get_touch_loc(void);