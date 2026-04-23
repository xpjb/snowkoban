#include "tile.hpp"

tile_prototype tile_prototype_get(int i) {
    static tile_prototype tile_prototypes[NUM_TT];
    tile_prototypes[T_SNOW] = (tile_prototype){"snow", " ptbc", {16, 0, 16, 16}, CS_SNOW_FOOTSTEP};
    tile_prototypes[T_ICE] = (tile_prototype){"ice", "/PTBC", {32, 16, 16, 16}, CS_SLIP};
    tile_prototypes[T_HOLE] = (tile_prototype){"hole", "h", {112, 0, 16, 16}, CS_NONE};
    tile_prototypes[T_WALL] = (tile_prototype){"wall", "#", {0, 0, 16, 16}, CS_NONE};
    tile_prototypes[T_NONE] = (tile_prototype){"none", "", {112, 0, 16, 16}, CS_NONE};
    tile_prototypes[T_WATER] = (tile_prototype){"water", "w", {128, 16, 16, 16}, CS_NONE};

    return tile_prototypes[i];
}

tile_prototype tile_prototype_get(tile t) {
    return tile_prototype_get((int)t);
}