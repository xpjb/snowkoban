#include "gef.hpp"
#include "util.hpp"
#include <math.h>
#include "coolmath.hpp"

#define max(A,B) (A > B ? A : B)

void snowflake_draw(gef_context *gc, int x, int y, int s) {
    const colour snowflake_colour = gef_rgb(255,255,255);
    gef_draw_rect(gc, snowflake_colour, x + s, y, s, s);
    gef_draw_rect(gc, snowflake_colour, x, y + s, s, s);
    gef_draw_rect(gc, snowflake_colour, x + s, y + s + s, s, s);
    gef_draw_rect(gc, snowflake_colour, x + s + s, y + s, s, s);
}

void snowflakes_draw(gef_context *gc, float t, int xo) {
    const int pixel_size = max(gc->xres / 400, 1); // why const auto doesnt draw snow?
    const auto spacing = pixel_size * 3;
    const auto starting_y = -spacing;
    const auto slowness = 12.f;
    const auto frequency = 10.f;
    const auto sin_magnitude = 10.f * (float)gc->xres / (float)2000;
    const auto vxh_magnitude = 100.f * (float) gc->xres / (float)2000;
    const auto potential_xmax = sin_magnitude + vxh_magnitude;
    
    const auto start_seed = (int)-(potential_xmax/spacing) - 1 + (xo / spacing);
    const auto end_seed = (int)(potential_xmax + gc->xres)/spacing + 1 + (xo / spacing);
    
    for (int seed = start_seed; seed < end_seed; seed++) {
        const auto phase = hash_floatn(seed, 0, slowness);
        const auto tprime = (t + phase) / slowness;
        const auto h = cm_frac(tprime);
        const auto iteration = cm_floor(tprime);

        const auto vxh = hash_floatn(seed + 9853*iteration, -vxh_magnitude, vxh_magnitude);
        const auto sin_phase = hash_floatn(seed + 1209*iteration, 0, 2*M_PI);

        const auto starting_x = seed * spacing;

        const auto x = lround(starting_x + h * vxh + sin_magnitude * sin(h * frequency + sin_phase) - xo);
        const auto y = lround(h * (gc->yres - starting_y) + starting_y);

        snowflake_draw(gc, x, y, pixel_size);
    }
}

