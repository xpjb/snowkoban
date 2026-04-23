#pragma once

#include <SDL2/SDL.h>
#include "gef.hpp"
#include "entity.hpp"
#include "tile.hpp"
#include "vla.hpp"
#include "grid.hpp"

struct level_prototype {
    vla<entity> starting_entities;
    grid<tile> tiles;

    bool complete = false;
    
    SDL_Texture *preview;
    char* title;

    level_prototype(const char* level_str, gef_context *gc);
};

struct world {
    const char *name;
    vla<level_prototype> lps;
    int total_presents = 0;
    int num_presents_collected = 0;

    colour pane_colour;

    world(){};

    world(const char *name, const char** level_strs, int n_levels, gef_context *gc);
};