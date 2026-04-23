#pragma once

#include "grid.hpp"
#include <SDL2/SDL.h>
#include "gef.hpp"
#include "entity.hpp"
#include "audio.hpp"
#include "scene.hpp"
#include "vla.hpp"
#include "tile.hpp"
#include "entity.hpp"


struct level {   
    bool initialized = false; // jank
    
    grid<tile> tiles;
    vla<entity> entities;
    bool player_faces_left = false;

    const char *title;

    level(){};
    level(const level_prototype *lp);
};

void level_destroy(level *l);
void level_draw(level *l, gef_context *gc, int xo, int yo, int pxsize, float t, float time);
bool level_move_entity(level *l, int entity_idx, int dx, int dy, audio *a);
bool level_check_victory(level *l);
bool level_do_ice(level *l, audio *a);

void level_step(level *l);