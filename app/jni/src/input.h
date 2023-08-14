#pragma once

#include "common.h"

typedef struct {
        SDL_Event event;
        SDL_Point touch_loc;
} input_context;

input_context *input_get_context(void);
void input_poll_events(void);
void input_handle_event(void);