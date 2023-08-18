#include "input.h"
#include "render.h"
#include "game/game.h"
#include "game/entity.h"

static struct Input input;

void input_poll_events(void)
{  
        input.touch_event = false;

        while (SDL_PollEvent(&input.event)) { 

                if (input.event.type == SDL_QUIT) {                  
                        game_change_state(0);
                }
                
                if (input.event.type == SDL_FINGERDOWN) {
                        input.touch_loc.x = render_get_w() * input.event.tfinger.x;
                        input.touch_loc.y = render_get_h() * input.event.tfinger.y;
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