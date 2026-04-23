#include "main_menu.hpp"
#include "util.hpp"
#include "snowflakes.hpp"
#include "audio.hpp"
#include "draw_level.hpp"
#include <stdbool.h>
#include <functional>

const auto num_buttons = 3;
const std::function<void(shared_data *)> btn_callbacks[] = {
    [](shared_data *app_d){app_d->set_scene(SCENE_LEVEL_MENU, TRANS_WIPE_RIGHT, trans_wipe_time);},
    [](shared_data *app_d){app_d->set_scene(SCENE_SETTINGS_MENU, TRANS_WIPE_RIGHT, trans_wipe_time);},
    [](shared_data *app_d){app_d->keep_going = false;},
};

void main_menu::handle_input(shared_data *app_d, SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            app_d->keep_going = false;
            return;
        }

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        if (up) {
            if (selection > 0) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection--;
            }
        } else if (down) {
            if (selection < 2) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection++;
            }
        } else if (select) {
            audio_play(&app_d->a, CS_MENU_SELECT);
            btn_callbacks[selection](app_d);
        }
    }/* else if (e.type == SDL_MOUSEMOTION) {
        for (int i = 0; i < num_buttons; i++) {
            const auto pane_rect = rect::centered(app_d->gc.xres/2, app_d->gc.yres/2, 0.8 * app_d->gc.xres, 0.8 * app_d->gc.yres);
            const auto r = rect::centered_layout(pane_rect, 0.8*pane_rect.w, 0.2*pane_rect.h, 1,  num_buttons, 0, i);
            if (r.contains(e.motion.x, e.motion.y)) {
                if (selection != i) {
                    audio_play(&app_d->a, CS_MENU_MOVE);
                    selection = i;
                }
            }
        }
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        for (int i = 0; i < num_buttons; i++) {
            const auto pane_rect = rect::centered(app_d->gc.xres/2, app_d->gc.yres/2, 0.8 * app_d->gc.xres, 0.8 * app_d->gc.yres);
            const auto r = rect::centered_layout(pane_rect, 0.8*pane_rect.w, 0.2*pane_rect.h, 1,  num_buttons, 0, i);
            if (r.contains(e.motion.x, e.motion.y)) {
                selection = i;
                btn_callbacks[selection](app_d);
                audio_play(&app_d->a, CS_MENU_SELECT);
            }
        }
    }*/
}

void main_menu::draw(shared_data *app_d, double dt) {
    // this fucntioin draws the main  menu
    const auto px_per_tile = 16*8;
    fill_background(&app_d->gc, app_d->gc.xres, app_d->gc.yres, px_per_tile, 0, 0);
    
    if (app_d->draw_snow) {
        snowflakes_draw(&app_d->gc, app_d->interp_time, app_d->snow_xo);
    }

    const auto pane_rect = rect::centered(app_d->gc.xres/2, app_d->gc.yres/2, 0.8 * app_d->gc.xres, 0.8 * app_d->gc.yres);

    for (int i = 0; i < num_buttons; i++) {
        const auto r = rect::centered_layout(pane_rect, 0.8*pane_rect.w, 0.2*pane_rect.h, 1,  num_buttons, 0, i);
        const auto line_colour = selection == i ?
            app_d->game_style.highlight:
            app_d->game_style.btn_line_colour;
        gef_draw_rect(&app_d->gc, line_colour, r.dilate(app_d->game_style.line));
        gef_draw_rect(&app_d->gc, app_d->game_style.btn_colour, r);

        const char *text[] = {
            "play",
            "settings",
            "quit",
        };
        gef_draw_bmp_text_centered(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, text[i], r.center().x, r.center().y);
    }
}