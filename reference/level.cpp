#include "level.hpp"

#include "coolmath.hpp"

tile_prototype tile_prototypes[NUM_TT];

level::level(const level_prototype *lp) {
    initialized = true;
    entities = lp->starting_entities.deep_copy();
    tiles = lp->tiles.deep_copy();
    title = lp->title;
}

// t from 0 to 1
void level_draw(level *l, gef_context *gc, int xo, int yo, int pxsize, float t, float time) {
    const int spritesheet_size = 16;
    const int tsize = spritesheet_size * pxsize;
    const float reflection_aspect_ratio = 1;
    const SDL_Rect ice_bg_clip = {32, 0, 16, 16};
    const SDL_Rect water_bg_clip = {128, 0, 16, 16};

    int player_frame = cm_frac(time) > 0.5;

    // first draw transparent backgrounds
    for (int i = 0; i < l->tiles.w; i++) {
        for (int j = 0; j < l->tiles.h; j++) {
            const auto t = l->tiles.get(i,j);
            if (t == T_ICE) {
                SDL_Rect to_rect = {xo + tsize * i, yo + tsize * j, tsize, tsize};
                gef_draw_sprite(gc, ice_bg_clip, to_rect);
            } else if (t == T_WATER) {
                SDL_Rect to_rect = {xo + tsize * i, yo + tsize * j, tsize, tsize};
                gef_draw_sprite(gc, water_bg_clip, to_rect);
            }
        }
    }
    // then draw sunk entities
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->is_sunk) {
            SDL_Rect to_rect = {
                lround(xo + tsize * cm_lerp(e->x - e->previous_dx, e->x, t)),
                lround(yo + tsize * cm_lerp(e->y - e->previous_dy, e->y, t)),
                tsize, tsize
            };
            SDL_Rect from_rect = entity_prototype_get(e->et).clip;
            gef_draw_sprite_ex(gc, from_rect, to_rect, 0, SDL_FLIP_NONE);
        }
    }

    // then draw entity reflections
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->is_sunk) continue;
        if (e->et != ET_TARGET && e->et != ET_NONE) {

            int reflection_offset = tsize - (2*pxsize*entity_prototype_get(e->et).base_h);
            SDL_Rect to_rect = {
                lround(xo + tsize * cm_lerp(e->x - e->previous_dx, e->x, t)),
                lround(yo + tsize * cm_lerp(e->y - e->previous_dy, e->y, t) + reflection_offset),
                tsize, tsize * reflection_aspect_ratio
            };
            SDL_Rect from_rect = entity_prototype_get(e->et).clip;
            SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
            if (e->et == ET_PLAYER) {
                if (!l->player_faces_left) {
                    flip = (SDL_RendererFlip)(((int)flip) | ((int)SDL_FLIP_HORIZONTAL));
                }
                
                from_rect.y += player_frame * spritesheet_size;
            }
            gef_draw_sprite_ex(gc, from_rect, to_rect, 0, flip);
        }
    }

    // then draw tiles incl. ice fg    
    for (int i = 0; i < l->tiles.w; i++) {
        for (int j = 0; j < l->tiles.h; j++) {
            SDL_Rect to_rect = {xo + tsize * i, yo + tsize * j, tsize, tsize};
            auto tp = tile_prototype_get(l->tiles.get(i, j));
            gef_draw_sprite(gc, tp.clip, to_rect);
        }
    }

    // then draw entities

    // draw order: first draw receptacles
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et == ET_TARGET) {
            SDL_Rect to_rect = {
                lround(xo + tsize * cm_lerp(e->x - e->previous_dx, e->x, t)),
                lround(yo + tsize * cm_lerp(e->y - e->previous_dy, e->y, t)),
                tsize, tsize
            };
            gef_draw_sprite(gc, entity_prototype_get(e->et).clip, to_rect);

        }
    }

    // then draw other entity types
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->is_sunk) continue;
        if (e->et != ET_TARGET && e->et != ET_NONE) {
            SDL_Rect to_rect = {
                lround(xo + tsize * cm_lerp(e->x - e->previous_dx, e->x, t)),
                lround(yo + tsize * cm_lerp(e->y - e->previous_dy, e->y, t)),
                tsize, tsize
            };
            SDL_Rect from_rect = entity_prototype_get(e->et).clip;
            SDL_RendererFlip flip = SDL_FLIP_NONE;
            if (e->et == ET_PLAYER) {
                if (!l->player_faces_left) {
                    //flip |= SDL_FLIP_HORIZONTAL;
                    flip = (SDL_RendererFlip)(((int)flip) | ((int)SDL_FLIP_HORIZONTAL));
                }
                from_rect.y += player_frame * spritesheet_size;
            }
            gef_draw_sprite_ex(gc, from_rect, to_rect, 0, flip);
        }
    }
}

bool level_is_entity_at(level *l, entity_type et, int x, int y) {
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et == et && e->x == x && e->y == y) {
            return true;
        }
    }
    return false;
}

void level_destroy(level *l) {
    if (!l->initialized) {
        return;
    }
    l->tiles.destroy();

    l->entities.destroy();
}

bool level_check_victory(level *l) {
    return l->entities.all([l](entity e1) {
        return e1.et != ET_TARGET || l->entities.any([e1](entity e2) {
            return e2.et == ET_PRESENT && e2.x == e1.x && e2.y == e1.y;
        });
    });
}

// its kind of an invariant that theres only one 'moveable' entity at a location
// really the receptacle has nothing in common with the other slidy boys
int level_get_movable_entity_index_at(level *l, int x, int y) {
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et != ET_TARGET && !e->is_sunk && e->x == x && e->y == y) {
            return i;
        }
    }
    return -1;
}

bool level_can_move_entity(level *l, int entity_idx, int dx, int dy) {
    entity *e = &l->entities.items[entity_idx];
    if (e->is_sunk) return false;
    int dest_x = e->x + dx;
    int dest_y = e->y + dy;
    tile t = l->tiles.get(dest_x, dest_y);
    if (t == T_NONE || t == T_WALL) {
        return false;
    }
    int dest_entity_idx = level_get_movable_entity_index_at(l, dest_x, dest_y);
    if (dest_entity_idx == -1 || level_can_move_entity(l, dest_entity_idx, dx, dy)) {
        return true;
    }
    return false;    
}

// stop doing it recursively, just do 1 step at a time, then it will animate easily
// and avoid the recursive sound tangly mess situation
// player cant move while stuff happening
bool level_move_entity(level *l, int entity_idx, int dx, int dy, audio *a) {
    if (!level_can_move_entity(l, entity_idx, dx, dy)) {
        return false;
    }
    entity *e = &l->entities.items[entity_idx];
    e->dx = dx;
    e->dy = dy;
    int dest_x = e->x + dx;
    int dest_y = e->y + dy;
    tile t = l->tiles.get(dest_x, dest_y);

    audio_play(a, tile_prototypes[t].walk_sound);

    if (t == T_NONE || t == T_WALL) {
        return false;
    }
    int dest_entity_idx = level_get_movable_entity_index_at(l, dest_x, dest_y);
    // ordering matters for these
    if (dest_entity_idx != -1 && level_move_entity(l, dest_entity_idx, dx, dy, a)) { // entity thats able to be moved
        return true;
    }
    if (e->et == ET_PRESENT && 
            l->entities.any([dest_x, dest_y](entity e) {return e.x == dest_x && e.y == dest_y && e.et == ET_TARGET;})) {
        audio_play(a, CS_MOVEFINAL);
    }
    if (dest_entity_idx == -1) return true; // empty
    return false;
}

void level_step(level *l) {
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
     
        e->x += e->dx;
        e->y += e->dy;

        if (l->tiles.get(e->x, e->y) == T_WATER) {
            bool sunk_entity_here_already = false;
            for (int j = 0; j < l->entities.length; j++) {
                entity *e2 = &l->entities.items[j];
                if (e2->x == e->x && e2->y == e->y && e2->is_sunk) {
                    sunk_entity_here_already = true;
                }
            }

            if (!sunk_entity_here_already) {
                e->is_sunk = true;
            }
        }

        e->previous_dx = e->dx;
        e->previous_dy = e->dy;

        e->dx = 0;
        e->dy = 0;
    };
}

bool level_do_ice(level *l, audio *a) {
    bool any_ice = false;
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];

        if (e->previous_dx == 0 && e->previous_dy == 0) {
            continue;
        }

        if (l->tiles.get(e->x, e->y) == T_ICE && level_can_move_entity(l, i, e->previous_dx, e->previous_dy)) {
            level_move_entity(l, i, e->previous_dx, e->previous_dy, a);
            any_ice = true;
        } else {
            //e->dx = 0;
            //e->dy = 0;
        }

    }
    return any_ice;
}
