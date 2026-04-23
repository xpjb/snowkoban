#include "application.hpp"
#include "util.hpp"
#include "instructions.hpp"
#include "level_menu.hpp"
#include "coolmath.hpp"
#define len(X) (sizeof(X) / sizeof(X[0]))

#define min(A,B) (A < B ? A : B)
#define max(A,B) (A > B ? A : B)

application::application(int xres, int yres) : m_shared_data{shared_data(xres, yres, "Snowkoban!")} {
    scenes[SCENE_INSTRUCTIONS] = new instructions();
    scenes[SCENE_MAIN_MENU] = new main_menu();
    scenes[SCENE_SETTINGS_MENU] = new settings_menu();
    scenes[SCENE_LEVEL_MENU] = new level_menu();
    scenes[SCENE_GAME] = new game();
}

void application::update(double dt) {
    if (m_shared_data.next_scene != SCENE_NONE) {
        if (m_shared_data.do_on_focus) {
            scenes[m_shared_data.next_scene]->on_focus(&m_shared_data);
            m_shared_data.do_on_focus = false;
        }
        m_shared_data.trans_t_current += dt;
        if (m_shared_data.trans_t_current > m_shared_data.trans_t_max) {
            scenes[m_shared_data.current_scene]->on_finish_transition(&m_shared_data);
            m_shared_data.current_scene = m_shared_data.next_scene;
            m_shared_data.next_scene = SCENE_NONE;
            m_shared_data.trans_t_current = 0;
            m_shared_data.trans_t_max = 0;

        }
    }
    m_shared_data.time += dt;
    m_shared_data.abs_time += dt;
    m_shared_data.interp_time += dt;
    m_shared_data.interp_time += min(2*dt, 0.1 * (m_shared_data.time - m_shared_data.interp_time));
    
    scenes[m_shared_data.current_scene]->update(&m_shared_data, dt);

    if (!Mix_PlayingMusic()) {
        m_shared_data.a.play_next_music();
    }

    return;
}

void application_draw_transition(SDL_Texture *current, SDL_Texture *next, float t, transition_type transition, gef_context *gc) {
    if (transition == TRANS_WIPE_RIGHT) {
        const auto x_offset = gc->xres * cm_slow_start_stop3(t);
        const auto from_rect = rect(0, 0, gc->xres, gc->yres).sdl_rect();
        const auto to_rect_current = rect(-x_offset, 0, gc->xres, gc->yres).sdl_rect();
        const auto to_rect_next = rect(gc->xres-x_offset, 0, gc->xres, gc->yres).sdl_rect();
        SDL_RenderCopyEx(gc->renderer, current, &from_rect, &to_rect_current, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gc->renderer, next, &from_rect, &to_rect_next, 0, NULL, SDL_FLIP_NONE);
    } else if (transition == TRANS_WIPE_LEFT) {
        const auto x_offset = gc->xres * cm_slow_start_stop3(t);
        const auto from_rect = rect(0, 0, gc->xres, gc->yres).sdl_rect();
        const auto to_rect_current = rect(x_offset, 0, gc->xres, gc->yres).sdl_rect();
        const auto to_rect_next = rect(x_offset - gc->xres, 0, gc->xres, gc->yres).sdl_rect();
        SDL_RenderCopyEx(gc->renderer, current, &from_rect, &to_rect_current, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gc->renderer, next, &from_rect, &to_rect_next, 0, NULL, SDL_FLIP_NONE);
    } 
}

void application::draw(double dt) {
    gef_clear(&m_shared_data.gc);

    if (m_shared_data.next_scene != SCENE_NONE) {
        const auto current_texture = SDL_CreateTexture(m_shared_data.gc.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_shared_data.gc.xres, m_shared_data.gc.yres);
        SDL_SetRenderDrawColor(m_shared_data.gc.renderer, 0, 0, 0, 255);
        SDL_SetRenderTarget(m_shared_data.gc.renderer, current_texture);
        SDL_RenderClear(m_shared_data.gc.renderer);
        scenes[m_shared_data.current_scene]->draw(&m_shared_data, dt);
        
        const auto next_texture = SDL_CreateTexture(m_shared_data.gc.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_shared_data.gc.xres, m_shared_data.gc.yres);
        SDL_SetRenderDrawColor(m_shared_data.gc.renderer, 0, 0, 0, 255);
        SDL_SetRenderTarget(m_shared_data.gc.renderer, next_texture);
        SDL_RenderClear(m_shared_data.gc.renderer);
        scenes[m_shared_data.next_scene]->draw(&m_shared_data, dt);

        SDL_SetRenderTarget(m_shared_data.gc.renderer, NULL);

        application_draw_transition(current_texture, next_texture, m_shared_data.trans_t_current / m_shared_data.trans_t_max, m_shared_data.transition, &m_shared_data.gc);
        gef_present(&m_shared_data.gc);
        SDL_DestroyTexture(current_texture);
        SDL_DestroyTexture(next_texture);
        
    } else {
        scenes[m_shared_data.current_scene]->draw(&m_shared_data, dt);

        gef_present(&m_shared_data.gc);
    }

}

void application::handle_input() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            m_shared_data.keep_going = false;
            return;
        } else {
            if (m_shared_data.next_scene == SCENE_NONE) {
                scenes[m_shared_data.current_scene]->handle_input(&m_shared_data, e);
            } else {
                scenes[m_shared_data.next_scene]->handle_input(&m_shared_data, e);
            }
            
        }
    }
}