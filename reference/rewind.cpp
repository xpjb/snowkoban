#include "rewind.hpp"
#include "coolmath.hpp"
#include "util.hpp"

// so have a number of horizontal bars
// variable velocity
// pos modded to screen
// slow start/stopped as well to look nice

// so you would have a number of cycles like with the snowflakes
// velocity would depend on cycle number
// for continuity probably have start x and end x actually

void rewind_effect(gef_context *gc, float t) {
    const int num_bars = 6;
    const int bar_h = 4;
    const colour bar_colour = gef_rgb(255,255,255);
    const colour overlay_colour = gef_rgba(128, 128, 128, 64);

    const float slowness = 10;

    const int spacial_repititions = 3;

    const SDL_Rect rewind_clip = {0, 16, 16, 16};

    const SDL_Rect to_rect = {32, 32, 64, 64};

    gef_draw_rect(gc, overlay_colour, 0, 0, gc->xres, gc->yres);

    for (int i = 0; i < num_bars; i++) {
        float phase = hash_floatn(i, 0, slowness);
        float tprime = (t + phase) / slowness;

        int iteration = cm_floor(tprime);

        int starty = hash_intn(9875 * i + 890734 * iteration, 0, spacial_repititions * gc->yres);
        int endy = hash_intn(9875 * i + 890734 * (iteration+1), 0, spacial_repititions * gc->yres);

        int y = ((int)cm_lerp(starty, endy, cm_slow_start_stop2(cm_frac(tprime)))) % gc->yres;

        gef_draw_rect(gc, bar_colour, 0, y, gc->xres, bar_h);
    }

    if (cm_frac(t) < 0.5) {
        gef_draw_sprite(gc,  rewind_clip, to_rect);
    }
}