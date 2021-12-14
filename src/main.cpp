#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "display.h"
#include "vector.h"

const u32 CUBE_DIM = 9;
const u32 N_POINTS = CUBE_DIM * CUBE_DIM * CUBE_DIM;
vec3 cube_points[N_POINTS];
vec2 projected_points[N_POINTS];

vec3 camera_pos = {.x = 0, .y = 0, .z = -5};
vec3 cube_rotation = {.x = 0, .y = 0, .z = 0};

f32 fov_factor = 640.0;

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
                vec3 new_point = {.x = x, .y = y, .z = z};
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

vec2 project(vec3 point)
{
    vec2 projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z
    };
    return projected_point;
}

void update(void)
{
    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;    
    for (int i = 0; i < N_POINTS; i++)
    {
        vec3 point = cube_points[i];
        vec3 point2 = rotate_y(point, cube_rotation.y);
        vec3 transformed_point = rotate_x(point2, cube_rotation.x);
        transformed_point.z -= camera_pos.z;
        vec2 projected_point = project(transformed_point);
        projected_points[i] = projected_point;
    }
}

void render(void)
{
    draw_grid();
    for (int i = 0; i < N_POINTS; i++)
    {
        vec2 p = projected_points[i];
        draw_rect(p.x + window_width / 2, p.y + window_height / 2, 4, 4, 0xFFFFFF00);
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);
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