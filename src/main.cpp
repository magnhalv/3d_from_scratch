#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "display.h"
#include "vector.h"

const u32 CUBE_DIM = 9;
const u32 N_POINTS = CUBE_DIM * CUBE_DIM * CUBE_DIM;
vec3 cube_points[N_POINTS];

bool is_running;

void setup(void)
{
    color_buffer = (u32 *)malloc(sizeof(u32) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    int point_count = 0;
    for (f32 x = -1.0; x <= 1.0; x += 0.25)
    {
        for (f32 y = -1.0; y <= 1.0; y += 0.25)
        {
            for (f32 z = -1.0; z <= 1.0; z += 0.25)
            {
                vec3 new_point = { .x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            is_running = false;
        }
        break;
    default:
        break;
    }
}

void update(void)
{
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_color_buffer();
    draw_grid();
    draw_rect(20, 20, 20, 20, 0xFFFF0000);
    // clear_color_buffer(0xFFFFFF00);
    SDL_RenderPresent(renderer);
}

int main(void)
{
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}