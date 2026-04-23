#include "instructions.hpp"

void instructions::draw(shared_data *app_d, double dt) {
    gef_draw_rect(&app_d->gc, gef_rgb(0, 0, 0), rect(0, 0, app_d->gc.xres, app_d->gc.yres));
    gef_draw_bmp_text(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, "wasd / arrows - move", app_d->gc.xres * 0.1, app_d->gc.yres * 0.1);
    gef_draw_bmp_text(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, "space / enter - select", app_d->gc.xres * 0.1, app_d->gc.yres * 0.3);
    gef_draw_bmp_text(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, "esc - go back", app_d->gc.xres * 0.1, app_d->gc.yres * 0.5);
    gef_draw_bmp_text(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, "u - undo", app_d->gc.xres * 0.1, app_d->gc.yres * 0.7);
    gef_draw_bmp_text(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, "r - reset", app_d->gc.xres * 0.1, app_d->gc.yres * 0.9);
}

void instructions::handle_input(shared_data *app_d, SDL_Event e) {
    if (e.type == SDL_KEYUP) {
        app_d->set_scene(SCENE_MAIN_MENU, TRANS_NONE, 0);
    }
}