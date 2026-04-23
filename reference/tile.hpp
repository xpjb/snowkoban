#pragma once

#include <SDL2/SDL.h>
#include "audio.hpp"

typedef enum {
    T_NONE,
    T_SNOW,
    T_ICE,
    T_HOLE,
    T_WALL,
    T_WATER,
    NUM_TT,
} tile;

typedef struct {
    const char *name;
    const char *symbols;
    SDL_Rect clip;
    channel_sound walk_sound;
} tile_prototype;

tile_prototype tile_prototype_get(int i);
tile_prototype tile_prototype_get(tile t);