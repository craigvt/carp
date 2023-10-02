#include "audio.h"
#include "game.h"

static struct Audio audio;

void audio_init(void)
{
    if (game_get_state() == TITLE) {
        audio.title_music = Mix_LoadMUS("audio/carp_title.wav");
        audio.play_button = Mix_LoadWAV("audio/carp_play_button.wav");
    }

    if (game_get_state() == PLAY) {
        audio.play_music = Mix_LoadMUS("audio/carp_play.wav");
        audio.fish_click = Mix_LoadWAV("audio/carp_fish_click.wav");
        audio.turtle_click = Mix_LoadWAV("audio/carp_turtle_click.wav");
        audio.play_end = Mix_LoadWAV("audio/carp_play_end.wav");
    }

    if (game_get_state() == GAMEOVER) {
        audio.gameover_music = Mix_LoadMUS("audio/carp_gameover.wav");
        audio.play_button = Mix_LoadWAV("audio/carp_play_button.wav");
    }
}

void audio_title_music(void)
{
    Mix_PlayMusic(audio.title_music, -1);
}

void audio_play_music(void)
{
    Mix_PlayMusic(audio.play_music, -1);
}

void audio_gameover_music(void)
{
    Mix_PlayMusic(audio.gameover_music, -1);
}

void audio_play_button(void)
{
    Mix_PlayChannel(-1, audio.play_button, 0);
}

void audio_fish_click(void)
{
    Mix_PlayChannel(-1, audio.fish_click, 0);
}

void audio_turtle_click(void)
{
    Mix_PlayChannel(-1, audio.turtle_click, 0);
}

void audio_play_end(void)
{
    Mix_PlayChannel(-1, audio.play_end, 0);
}

void audio_destroy(void)
{
    if (game_get_state() == TITLE) {
	    Mix_FreeMusic(audio.title_music);
        Mix_FreeChunk(audio.play_button);
    }
    if (game_get_state() == PLAY) {
	    Mix_FreeMusic(audio.play_music);
        Mix_FreeChunk(audio.fish_click);
        Mix_FreeChunk(audio.turtle_click);
        Mix_FreeChunk(audio.play_end);
    }
    if (game_get_state() == GAMEOVER) {
	    Mix_FreeMusic(audio.gameover_music);
        Mix_FreeChunk(audio.play_button);
    }
}