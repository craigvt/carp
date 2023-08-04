#pragma once

#include "common.h"

typedef enum {
        TOUCH,
        CLEAR
} action;

typedef struct {
        SDL_Event event;
        SDL_Point touch_loc;
        action    action_id;
} input_context;

input_context *input_get_context(void);
void input_poll_events(void);