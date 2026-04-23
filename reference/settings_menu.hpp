#pragma once

#include "scene.hpp"
#include "gef.hpp"

struct settings_menu : scene {
    int selection = 0;
    void draw(shared_data *app_d, double dt);
    void update(shared_data *app_d, double dt) {}
    void on_focus(shared_data *app_d) {}
    void handle_input(shared_data *app_d, SDL_Event e);
};
