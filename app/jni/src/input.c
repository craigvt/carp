#include "input.h"
#include "render.h"
#include "game.h"
#include "entity.h"

static struct Input input;

void input_poll_events(void)
{  
        input.touch_event = false;

        while (SDL_PollEvent(&input.event)) { 

                if (input.event.type == SDL_QUIT) {                
                        game_set_new_state(0);
                }
                
                if (input.event.type == SDL_FINGERDOWN) {
                        SDL_Rect bounds;
                        SDL_GetDisplayBounds(0, &bounds);
                        
                        input.touch_loc.x = bounds.w * input.event.tfinger.x;
                        input.touch_loc.y = bounds.h * input.event.tfinger.y;
                        input.touch_event = true;
                }
        }
}

bool input_get_event(void)
{
        return input.touch_event;
}

SDL_Point input_get_touch_loc(void)
{
        return input.touch_loc;
}