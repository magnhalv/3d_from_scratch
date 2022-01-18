#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "types.h"

typedef struct {
    bool dot;
    bool wireframe;
    bool fill;  
    bool texture;  
} DrawOptions;

typedef struct {
    DrawOptions draw_options;
    bool enable_back_face_culling;
} RenderOptions;

extern u32 window_height;
extern u32 window_width;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern u32 *color_buffer;
extern f32 *z_buffer;
extern SDL_Texture *color_buffer_texture;
extern RenderOptions render_options;

bool initialize_window();
void setup();
void draw_grid();
void draw_line(i32 x0, i32 y0, i32 x1, i32 y1, u32 color);
void draw_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color);
void draw_rect(u32 x, u32 y, u32 width, u32 height, u32 color);
void render_color_buffer();
void clear_color_buffer(u32 color);
void clear_z_buffer();
void destroy_window();
inline void draw_pixel(u32 x, u32 y, u32 color);

#endif
