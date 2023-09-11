#pragma once

#include "common.h"

struct Audio {
    Mix_Music *title_music;
    Mix_Music *play_music;
    Mix_Music *gameover_music;
    Mix_Chunk *play_button;
    Mix_Chunk *fish_click;
    Mix_Chunk *turtle_click;
    Mix_Chunk *play_end;
    bool music_active;
};

void audio_init(void);
void audio_manage_music(void);
void audio_title_end(void);
void audio_fish_click(void);
void audio_turtle_click(void);
void audio_play_end(void);
void audio_destroy(void);
