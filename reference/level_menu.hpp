#pragma once

#include "scene.hpp"



struct level_menu : scene {
    void draw(shared_data *app_d, double dt);
    void update(shared_data *app_d, double dt);
    void on_focus(shared_data *app_d);
    void handle_input(shared_data *app_d, SDL_Event e);

    double target_x_offset = 0;
    double current_x_offset = 0;

    double target_y_offset = 0;
    double current_y_offset = 0;
    
    int x_viewport_offset(int x, int xmax, int xres);
    int y_viewport_offset(int y, int ymax, int yres);
};