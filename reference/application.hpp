#pragma once

#include "gef.hpp"
#include "scene.hpp"
#include "level_menu.hpp"
#include "main_menu.hpp"
#include "settings_menu.hpp"
#include "game.hpp"
#include <stdbool.h>

#define MAX_SCENES 16;


struct application {
    scene *scenes[NUM_SCENES];
    
    shared_data m_shared_data;

    application(int xres, int yres);
    void update(double dt);
    void draw(double dt);
    void handle_input();   
};