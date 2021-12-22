#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <vector>

#include "display.h"
#include "vector.h"
#include "mesh.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000/FPS)

std::vector<triangle> triangles_to_render;

vec3 camera_pos = {.x = 0, .y = 0, .z = -5};

f32 fov_factor = 640.0;

i32 previous_frame_time = 0;

bool is_running;

void setup(void)
{
    color_buffer = (u32 *)malloc(sizeof(u32) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    g_mesh = load_obj_file("./assets/f22.obj");
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
    g_mesh.rotation.x = 3.14;
    g_mesh.rotation.y += 0.01;
    g_mesh.rotation.z = -0.2;

    triangles_to_render.clear();

    for (unsigned int i = 0; i < g_mesh.faces.size(); i++) {
        face face = g_mesh.faces[i];
        vec3 face_verticies[3];
        face_verticies[0] = g_mesh.vertices[face.a];
        face_verticies[1] = g_mesh.vertices[face.b];
        face_verticies[2] = g_mesh.vertices[face.c];

        triangle projected_triangle;
        for (int j = 0; j < 3; j++) {
            vec3 transformed_vertex = face_verticies[j];

            transformed_vertex = rotate_x(transformed_vertex, g_mesh.rotation.x);
            transformed_vertex = rotate_y(transformed_vertex, g_mesh.rotation.y);
            transformed_vertex = rotate_z(transformed_vertex, g_mesh.rotation.z);

            transformed_vertex.z -= camera_pos.z;

            vec2 projected_point = project(transformed_vertex);

            projected_point.x += (window_height/2);
            projected_point.y += (window_height/2);

            projected_triangle.points[j] = projected_point;
        }
        triangles_to_render.push_back(projected_triangle);        
    }
}

void render(void)
{
    draw_grid();    

    int num_triangles = triangles_to_render.size();
    for (int i = 0; i < num_triangles; i++) {
        vec2 *points = triangles_to_render[i].points;        
        draw_triangle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, 0xFFFFFF00);
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

        i32 time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);  
        if (time_to_wait < 0) {
            fprintf(stdout, "Misssed frame target by %d ms.\n", time_to_wait);
        }
        else if (time_to_wait <= FRAME_TARGET_TIME) {
            SDL_Delay(time_to_wait);
        }        
        previous_frame_time = SDL_GetTicks();
    }

    destroy_window();

    return 0;
}