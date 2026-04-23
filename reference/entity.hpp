#pragma once

#include <SDL2/SDL.h>

typedef enum {
    ET_NONE,
    ET_PLAYER,
    ET_CRATE,
    ET_PRESENT,
    ET_TARGET,
    NUM_ET,
} entity_type;

typedef struct {
    const char *name;
    const char *symbols;
    SDL_Rect clip;
    bool moveable;
    int base_h;
} entity_prototype;

/*
previous dx and dy are weird, they are used for sliding and drawing.
its not really obvious why its got to be separate from normal dx and dy til you try implementing
its so level step can happen without just forgetting the sliding / moving direction basically
*/
typedef struct {
    entity_type et;
    int x;
    int y;
    int dx;
    int dy;
    int previous_dx;
    int previous_dy;
    bool is_sunk = false;
} entity;

entity_prototype entity_prototype_get(entity_type et);
