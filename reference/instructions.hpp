#pragma once

#include "scene.hpp"
struct instructions : scene {
    void draw(shared_data *app_d, double dt);
    void update(shared_data *app_d, double dt) {}
    void on_focus(shared_data *app_d) {}
    void handle_input(shared_data *app_d, SDL_Event e);
};