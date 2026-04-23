#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "rect.hpp"

#define gef_die(G, X) printf("%s %d %s: dying -- %s (%s)\n", __FILE__, __LINE__, __func__, X, SDL_GetError()), gef_teardown(G)

typedef struct { 
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *atlas;
    unsigned int xres;
    unsigned int yres;
    int64_t tstart;

} gef_context;

typedef struct {
    TTF_Font *gfont;
} font_handle;

typedef struct {
    SDL_Texture *texture;
    int w;
    int h;
    int cx;
    int cy;
} bmp_font;

typedef struct {
    int scale;
    int spacing;
} bmp_font_settings;

bmp_font gef_load_bmp_font(gef_context *gc, const char *path, int cx, int cy);

SDL_Point gef_bmp_font_size(bmp_font f, bmp_font_settings fs, int nchar);
void gef_draw_bmp_text(gef_context *gc, bmp_font f, bmp_font_settings fs, const char *text, int x, int y);
void gef_draw_bmp_text_centered(gef_context *gc, bmp_font f, bmp_font_settings fs, const char *text, int x, int y);

typedef struct {
    SDL_Texture *texture;
    SDL_Texture *texture_shadow;
    int w;
    int h;
} text_handle;

typedef struct {
    int r;
    int g;
    int b;
    int a;
} colour;

colour gef_rgb(int r, int g, int b);
colour gef_rgba(int r, int g, int b, int a);

rect gef_screen_rect(gef_context *gc);

gef_context gef_init(const char *name, int xres, int yres);
void gef_load_atlas(gef_context *gc, const char *path);
void gef_draw_sprite(gef_context *gc, SDL_Rect clip, SDL_Rect to_rect);
void gef_draw_sprite_ex(gef_context *gc, SDL_Rect clip, SDL_Rect to_rect, double angle, SDL_RendererFlip flip);
void gef_teardown(gef_context *gc);

// drawing primitives
void gef_draw_pixel(gef_context *gc, colour c, int x, int y);
void gef_draw_rect(gef_context *gc, colour c, int x, int y, int w, int h);
void gef_draw_rect(gef_context *gc, colour c, rect r);

void gef_start_frame(gef_context *gc);
void gef_end_frame(gef_context *gc);

void gef_clear(gef_context *gc);
void gef_present(gef_context *gc);

font_handle gef_load_font(const char *path, int size);
text_handle gef_make_text(gef_context *gc, font_handle f, const char *text, int r, int g, int b);
void gef_draw_text(gef_context *gc, text_handle text, int x, int y);
void gef_destroy_text(text_handle text);