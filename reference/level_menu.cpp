#include "level_menu.hpp"
#include "coolmath.hpp"
#include "snowflakes.hpp"
#include "draw_level.hpp"

#define util_min(A,B) (A < B ? A : B)
#define util_max(A,B) (A > B ? A : B)


void level_menu::update(shared_data *app_d, double dt) {
    const auto snappiness = 8;
    current_x_offset += (target_x_offset - current_x_offset) * dt * snappiness;
    current_y_offset += (target_y_offset - current_y_offset) * dt * snappiness;

    app_d->time_since_select += dt;
}

int level_menu::x_viewport_offset(int x, int xmax, int xres) {
    const auto btn_space_x = 20;
    const auto side = 200;

    return xres * 0.05 + btn_space_x - cm_clamp(0, x - 3, xmax - 5)*(btn_space_x + side);
}

int level_menu::y_viewport_offset(int y, int ymax, int yres) {
    const auto btn_space_y = 70;
    const auto side = 200;

    return yres * 0.2 - side/2 - cm_clamp(0, y - 1, ymax - 3) *(btn_space_y + side);
}

void level_menu::on_focus(shared_data *app_d) {

    const auto num_levels_x = app_d->worlds.max([](world w){return w.lps.length;});
    const auto num_levels_y = app_d->worlds.length;

    const auto xo = x_viewport_offset(app_d->level_idx, num_levels_x, app_d->gc.xres);
    const auto yo = y_viewport_offset(app_d->world_idx, num_levels_y, app_d->gc.yres);

    target_x_offset = xo;
    current_x_offset = xo;
    target_y_offset = yo;
    current_y_offset = yo;
}

void level_menu::draw(shared_data *app_d, double dt) {
    const auto btn_space_x = 20;
    const auto btn_space_y = 70;
    const auto side = 200;
    
    const auto px_per_tile = 16*8;
    fill_background(&app_d->gc, app_d->gc.xres, app_d->gc.yres, px_per_tile, 0, 0);
    
    const auto num_levels_x = app_d->worlds.max([](world w){return w.lps.length;});
    const auto num_levels_y = app_d->worlds.length;
    
    target_x_offset = x_viewport_offset(app_d->level_idx, num_levels_x, app_d->gc.xres);
    target_y_offset = y_viewport_offset(app_d->world_idx, num_levels_y, app_d->gc.yres);

    if (app_d->draw_snow) {
        snowflakes_draw(&app_d->gc, app_d->time, app_d->snow_xo - current_x_offset);
    }

    // draw normal levels
    for (int i = 0; i < app_d->worlds.length; i++) {
        for (int j = 0; j < app_d->worlds.items[i].lps.length; j++) {
            const auto btn_rect = rect(
                current_x_offset + (btn_space_x+side) * j, 
                current_y_offset + (btn_space_y+side) * i, 
                side, side);

            const auto border_rect = btn_rect.dilate(app_d->game_style.line);
            const auto border_colour = app_d->worlds.items[i].lps.items[j].complete ?
                    app_d->game_style.secondary :
                    app_d->game_style.btn_line_colour;

            gef_draw_rect(&app_d->gc, border_colour, border_rect);
            const auto r = btn_rect.sdl_rect();
            SDL_RenderCopy(app_d->gc.renderer, app_d->worlds.items[i].lps.items[j].preview, NULL, &r);

            const auto vignette_colour = app_d->worlds.items[i].lps.items[j].complete ?
                gef_rgba(0, 0, 0, 0) :
                gef_rgba(0, 0, 0, 128);

            gef_draw_rect(&app_d->gc, vignette_colour, btn_rect);
        }
    }

    // draw selected level
    const auto pop_time = 0.25;

    for (int i = 0; i < app_d->worlds.length; i++) {
        if (app_d->world_idx != i) continue;
        for (int j = 0; j < app_d->worlds.items[i].lps.length; j++) {
            if (app_d->level_idx != j) continue;

            const auto dilate_amt = cm_slow_stop2(util_min(app_d->time_since_select/pop_time, 1)) * 20;
            const auto btn_rect = rect(
                current_x_offset + (btn_space_x+side) * j, 
                current_y_offset + (btn_space_y+side) * i, 
                side, side)
                .dilate(dilate_amt);

            const auto border_rect = btn_rect.dilate(app_d->game_style.line);
        const auto border_colour = app_d->game_style.highlight;

            gef_draw_rect(&app_d->gc, border_colour, border_rect);
            const auto r = btn_rect.sdl_rect();
            SDL_RenderCopy(app_d->gc.renderer, app_d->worlds.items[i].lps.items[j].preview, NULL, &r);

            const auto vignette_colour = app_d->worlds.items[i].lps.items[j].complete ?
                gef_rgba(0, 0, 0, 0) :
                gef_rgba(0, 0, 0, 128);

            gef_draw_rect(&app_d->gc, vignette_colour, btn_rect);
        }
    }

    // draw world titles
    for (int i = 0; i < app_d->worlds.length; i++) {
        gef_draw_bmp_text(&app_d->gc, app_d->game_style.game_font, app_d->game_style.small, app_d->worlds.items[i].name, 5 + app_d->gc.xres * 0.05, current_y_offset + (btn_space_y+side) * i - 35);
    }
}

bool previous_world(shared_data *app_d) {
    if (app_d->world_idx > 0) {
        app_d->world_idx--;
        app_d->level_idx = util_min(app_d->level_idx, app_d->current_world()->lps.length - 1);
        
        return true;
    }
    return false;
}

bool next_world(shared_data *app_d) {
    if (app_d->world_idx < app_d->worlds.length - 1) {
        app_d->world_idx++;
        app_d->level_idx = util_min(app_d->level_idx, app_d->current_world()->lps.length - 1);
        return true;
    }
    return false;
}

bool previous_level(shared_data *app_d) {
    if (app_d->level_idx > 0) {
        app_d->level_idx--;
        return true;
    }
    return false;
}

bool next_level(shared_data *app_d) {
    if (app_d->level_idx < app_d->current_world()->lps.length - 1) {
        app_d->level_idx++;
        return true;
    }
    return false;
}

void level_menu::handle_input(shared_data *app_d, SDL_Event e) {
    // move selected world and level and maybe scroll
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            app_d->set_scene(SCENE_MAIN_MENU, TRANS_WIPE_LEFT, trans_wipe_time);
        }

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool left = sym == SDLK_LEFT || sym == SDLK_a || sym == SDLK_h;
        bool right = sym == SDLK_RIGHT || sym == SDLK_d || sym == SDLK_l;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        const auto on_move_success = [&](){
            audio_play(&app_d->a, CS_MENU_MOVE);
            app_d->time_since_select = 0;
        };

        if (left) {
            if (previous_level(app_d)) {
                on_move_success();
            } else if (previous_world(app_d)) {
                app_d->level_idx = app_d->current_world()->lps.length - 1;
                on_move_success();
            }
        } else if (right) {
            if (next_level(app_d)) {
                on_move_success();
            } else if (next_world(app_d)) {
                on_move_success();
                app_d->level_idx = 0;
            }
        } else if (up) {
            if (previous_world(app_d)) {
                on_move_success();
            }
        } else if (down) {
            if (next_world(app_d)) {
                on_move_success();
            }
        }
        if (select) {
            audio_play(&app_d->a, CS_MENU_SELECT);
            app_d->set_scene(SCENE_GAME, TRANS_WIPE_RIGHT, trans_wipe_time);
        }
    }
}
