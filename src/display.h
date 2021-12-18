#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "types.h"

extern u32 window_height;
extern u32 window_width;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern u32 *color_buffer;
extern SDL_Texture *color_buffer_texture;

bool initialize_window();
void setup();
void draw_grid();
void draw_line(i32 x0, i32 y0, i32 x1, i32 y1, u32 color);
void draw_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 x3, u32 color);
void draw_rect(u32 x, u32 y, u32 width, u32 height, u32 color);
void render_color_buffer();
void clear_color_buffer(u32 color);
void destroy_window();
inline void set_pixel(u32 x, u32 y, u32 color);


#endif
