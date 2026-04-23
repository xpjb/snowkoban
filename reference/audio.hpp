#pragma once

#include <SDL2/SDL_mixer.h>
#include "vla.hpp"

typedef enum {
    CS_NONE,
    CS_SNOW_FOOTSTEP,
    CS_SLIP,
    CS_UNDO,
    CS_LOSE,
    CS_MOVEFINAL,
    CS_WIN,
    CS_MENU_MOVE,
    CS_MENU_SELECT,
    NUM_CS,
} channel_sound;

struct audio {
    vla<Mix_Music *>playlist = vla<Mix_Music*>();
    int curr_music = 0;
    Mix_Chunk *sounds[NUM_CS];
    float relative_volume[NUM_CS];
    float sfx_volume;
    float bgm_volume;
    void play_next_music();
};

audio audio_init();
void audio_play(audio *a, channel_sound s);
void audio_set_sfx_volume(audio *a, float v);
void audio_set_bgm_volume(audio *a, float v);
