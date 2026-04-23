#pragma once

#include "gef.hpp"
#include "vla.hpp"
#include "grid.hpp"
#include "util.hpp"
#include "tile.hpp"
#include "entity.hpp"

void draw_level(gef_context *gc, grid<tile> terrain, vla<entity> entities, int xres, int yres, int xo, int yo);
void fill_background(gef_context *gc, int xres, int yres, int px_per_tile, int xo, int yo);