#include "settings_menu.hpp"
#include "snowflakes.hpp"
#include "util.hpp"
#include "draw_level.hpp"

#define len(X) (sizeof(X) / sizeof(X[0]))

#define min(A,B) (A < B ? A : B)
#define max(A,B) (A > B ? A : B)

const int num_buttons = 6;

const std::function<void(shared_data *, bool, bool)> btn_callbacks[] = {
    [](shared_data *app_d, bool left, bool right){                
        app_d->draw_snow = !app_d->draw_snow;
    },
    [](shared_data *app_d, bool left, bool right){                
        if (left) {
            app_d->max_scale = max(app_d->max_scale - 1, 1);
        } else if (right) {
            app_d->max_scale = min(app_d->max_scale + 1, 20);
        }
    },
    [](shared_data *app_d, bool left, bool right){
        // res idk yet
    },
    [](shared_data *app_d, bool left, bool right){
        if (left) {
            audio_set_bgm_volume(&app_d->a, max(app_d->a.bgm_volume - 0.04, 0));
        } else if (right) {
            audio_set_bgm_volume(&app_d->a, min(app_d->a.bgm_volume + 0.04, 1));
        }
    },
    [](shared_data *app_d, bool left, bool right){
        //sfx volume
        if (left) {
            audio_set_sfx_volume(&app_d->a, max(app_d->a.sfx_volume - 0.04, 0));
        } else if (right) {
            audio_set_sfx_volume(&app_d->a, min(app_d->a.sfx_volume + 0.04, 1));
        }
    },
    [](shared_data *app_d, bool left, bool right){app_d->set_scene(SCENE_MAIN_MENU, TRANS_WIPE_LEFT, trans_wipe_time);},
};

void settings_menu::draw(shared_data *app_d, double dt) {
    const auto px_per_tile = 16*8;
    fill_background(&app_d->gc, app_d->gc.xres, app_d->gc.yres, px_per_tile, 0, 0);
    
    if (app_d->draw_snow) {
        snowflakes_draw(&app_d->gc, app_d->interp_time, app_d->snow_xo);
    }

    const auto pane_rect = rect::centered(app_d->gc.xres/2, app_d->gc.yres/2, 0.8 * app_d->gc.xres, 0.8 * app_d->gc.yres);
    
    const auto n_buttons = 6;

    char snow_buf[64] = {0};
    sprintf(snow_buf, "snow: %s", app_d->draw_snow ? "on" : "off");
    char scale_buf[64] = {0};
    sprintf(scale_buf, "max scale: %d", app_d->max_scale);
    char res_buf[64] = {0};
    sprintf(res_buf, "resolution: %dx%d", app_d->gc.xres, app_d->gc.yres);
    char music_vol_buf[64] = {0};
    sprintf(music_vol_buf, "music volume: %.0f%%", app_d->a.bgm_volume * 100);
    char sfx_vol_buf[64] = {0};
    sprintf(sfx_vol_buf, "sfx volume: %.0f%%", app_d->a.sfx_volume * 100);

    const char* text[n_buttons] = {
        snow_buf,
        scale_buf,
        res_buf,
        music_vol_buf,
        sfx_vol_buf,
        "back",
    };
    
    for (int i = 0; i < n_buttons; i++) {
        const auto r = rect::centered_layout(pane_rect, 0.8*pane_rect.w, 0.1*pane_rect.h, 1,  n_buttons, 0, i);
        const auto line_colour = selection == i ?
            app_d->game_style.highlight:
            app_d->game_style.btn_line_colour;
        gef_draw_rect(&app_d->gc, line_colour, r.dilate(app_d->game_style.line));
        gef_draw_rect(&app_d->gc, app_d->game_style.btn_colour, r);
        gef_draw_bmp_text_centered(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, text[i], r.center().x, r.center().y);
    }
}

void settings_menu::handle_input(shared_data *app_d, SDL_Event e) {
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


        if (up) {
            if (selection > 0) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection--;
            }
        } else if (down) {
            if (selection < num_buttons - 1) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection++;
            }
        } else if (select || left || right) {
            audio_play(&app_d->a, CS_MENU_SELECT);
            btn_callbacks[selection](app_d, left, right);
        }   
    } else if (e.type == SDL_MOUSEMOTION) {
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
                bool left = e.motion.x < r.center().x;
                bool right = !left;
                selection = i;
                btn_callbacks[selection](app_d, left, right);
                audio_play(&app_d->a, CS_MENU_SELECT);
            }
        }
    }
}