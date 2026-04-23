#include "draw_level.hpp"
#define len(X) (sizeof(X) / sizeof(X[0]))
#define min(A,B) (A < B ? A : B)
#define max(A,B) (A > B ? A : B)

void fill_background(gef_context *gc, int xres, int yres, int px_per_tile, int xo, int yo) {
    // Wall filler
    const auto n_spaces_x = ceil(xres / px_per_tile);
    const auto n_spaces_y = ceil(yres / px_per_tile);

    const auto filler_xo = (xo % px_per_tile) - px_per_tile;
    const auto filler_yo = (yo % px_per_tile) - px_per_tile;

    const auto clip_wall = tile_prototype_get(T_WALL).clip;

    for (int i = 0; i < n_spaces_x + 2; i++) {
        for (int j = 0; j < n_spaces_y + 2; j++) {
            const SDL_Rect to_rect = {filler_xo + i*px_per_tile, filler_yo + j*px_per_tile, px_per_tile, px_per_tile};
            gef_draw_sprite(gc, clip_wall, to_rect);
        }
    }    
}

void draw_level(gef_context *gc, grid<tile> terrain, vla<entity> entities, int xres, int yres, int xo, int yo) {
    const auto px_per_tile = min(
        xres / terrain.w,
        yres / terrain.h
    );

    // Wall filler
    const auto n_spaces_x = ceil(xres / px_per_tile);
    const auto n_spaces_y = ceil(yres / px_per_tile);

    const auto filler_xo = (xo % px_per_tile) - px_per_tile;
    const auto filler_yo = (yo % px_per_tile) - px_per_tile;

    const auto clip_wall = tile_prototype_get(T_WALL).clip;

    for (int i = 0; i < n_spaces_x + 2; i++) {
        for (int j = 0; j < n_spaces_y + 2; j++) {
            const SDL_Rect to_rect = {filler_xo + i*px_per_tile, filler_yo + j*px_per_tile, px_per_tile, px_per_tile};
            gef_draw_sprite(gc, clip_wall, to_rect);
        }
    }

    // Terrain
    const auto terrain_w = px_per_tile * terrain.w;
    const auto terrain_h = px_per_tile * terrain.h;

    const auto terrain_xo = xo + xres / 2 - terrain_w / 2;
    const auto terrain_yo = yo + yres / 2 - terrain_h / 2;

    for (int i = 0; i < terrain.w; i++) {
        for (int j = 0; j < terrain.h; j++) {
            const auto from_rect = tile_prototype_get(terrain.get(i, j)).clip;
            const auto to_rect = (SDL_Rect) {terrain_xo + i*px_per_tile, terrain_yo + j*px_per_tile, px_per_tile, px_per_tile};
            gef_draw_sprite(gc, from_rect, to_rect);
        }
    }

    // Entities
    for (int i = 0; i < entities.length; i++) {
        const auto from_rect = entity_prototype_get(entities.items[i].et).clip;
        const auto to_rect = (SDL_Rect) {terrain_xo + entities.items[i].x*px_per_tile, terrain_yo + entities.items[i].y*px_per_tile, px_per_tile, px_per_tile};
        gef_draw_sprite(gc, from_rect, to_rect);
    }
}