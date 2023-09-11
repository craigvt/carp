#include "system.h"

int system_init(void)
{
        if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
                LOGI("SDL Failed to Initialize: %s", SDL_GetError());
		return 1;
	}

        if ( IMG_Init( IMG_INIT_PNG ) < 0 ) {
                LOGI("SDL_Image Failed to Initialize: %s", SDL_GetError());
		return 1;
	}

        if ( TTF_Init() < 0 ) {
                LOGI("SDL_TTF Failed to Initialize: %s", SDL_GetError());
		return 1;
	}

        if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 ) {   
                LOGI("SDL_Mixer Failed to Initialize: %s", SDL_GetError());
                return 1; 
        }

        return 0;
}

void system_quit(void)
{
        IMG_Quit();
        TTF_Quit();
        Mix_CloseAudio();
        SDL_Quit();
}