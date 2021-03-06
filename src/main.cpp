#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "lib/upng.h"
#include "camera.h"
#include "clipping.h"

#define FPS 200
#define FRAME_TARGET_TIME (1000 / FPS)

std::vector<triangle> triangles_to_render;

i32 previous_frame_time = 0;
bool is_running;

Mat4 projection;
Mat4 world_matrix;
Mat4 view_matrix;

void setup(void)
{
    color_buffer = (u32 *)malloc(sizeof(u32) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    z_buffer = (f32 *)malloc(sizeof(f32) * window_width * window_height);

    f32 aspect_x = (f32)window_width / window_height;
    f32 aspect_y = (f32)window_height / window_width;
    f32 fov_y = M_PI / 3;
    f32 fov_x = atan((tan(fov_y / 2) * aspect_x)) * 2;
    f32 z_near = 0.1;
    f32 z_far = 100.0;
    projection = mat4_make_perspective(fov_y, aspect_y, z_near, z_far);
    init_frustum_planes(fov_x, fov_y, z_near, z_far);

    // mesh_texture = (u32*) REDBRICK_TEXTURE;

    g_mesh = load_obj_file("./assets/cube.obj");
    // load_cube_mesh();
    load_png_texture_data("./assets/f22.png");
}

void tear_down()
{
    free_texture();
}

void process_input(f32 dt)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
        {
            SDL_Keycode key = event.key.keysym.sym;
            if (key == SDLK_ESCAPE)
            {
                is_running = false;
            }
            else if (key == SDLK_1)
            {
                render_options.draw_options = {0};
                render_options.draw_options.dot = true;
                render_options.draw_options.wireframe = true;
            }
            else if (key == SDLK_2)
            {
                render_options.draw_options = {0};
                render_options.draw_options.wireframe = true;
            }
            else if (key == SDLK_3)
            {
                render_options.draw_options = {0};
                render_options.draw_options.fill = true;
            }
            else if (key == SDLK_4)
            {
                render_options.draw_options = {0};
                render_options.draw_options.wireframe = true;
                render_options.draw_options.fill = true;
            }
            else if (key == SDLK_5)
            {
                render_options.draw_options = {0};
                render_options.draw_options.texture = true;
            }
            else if (key == SDLK_6)
            {
                render_options.draw_options = {0};
                render_options.draw_options.texture = true;
                render_options.draw_options.wireframe = true;
            }
            else if (key == SDLK_b)
            {
                render_options.enable_back_face_culling = true;
            }
            else if (key == SDLK_x)
            {
                render_options.enable_back_face_culling = false;
            }

            // Movement
            if (key == SDLK_w)
            {
                // camera.forward_velocity = multiply(camera.direction, 5.0 * dt);
                // camera.position = add(camera.position, camera.forward_velocity);
                g_mesh.translation.z -= 10.0 * dt;
            }
            if (key == SDLK_s)
            {
                // camera.forward_velocity = multiply(camera.direction, -5.0 * dt);
                // camera.position = add(camera.position, camera.forward_velocity);
                g_mesh.translation.z += 10.0 * dt;
            }

            if (key == SDLK_a)
            {
                g_mesh.translation.x += 10.0 * dt;
                // camera.yaw += 1.0 * dt;
            }
            if (key == SDLK_d)
            {
                g_mesh.translation.x -= 10.0 * dt;
                // camera.yaw -= 1.0 * dt;
            }

            else if (key == SDLK_UP)
            {
                camera.position.y += 3 * dt;
            }
            else if (key == SDLK_DOWN)
            {
                camera.position.y -= 3 * dt;
            }

            break;
        }
        default:
            break;
        }
    }
}

vec3 get_normalv(vec4 points[3])
{
    vec3 a = subtract(to_vec3(points[1]), to_vec3(points[0]));
    normalize(&a);
    vec3 b = subtract(to_vec3(points[2]), to_vec3(points[0]));
    normalize(&b);

    vec3 normalv = cross(a, b);
    normalize(&normalv);
    return normalv;
}

void update(f32 dt)
{
    // g_mesh.rotation.x += 0.6 * dt;
    g_mesh.rotation.y = M_PI / 2;
    // g_mesh.rotation.z += 0.6 * dt;

    g_mesh.scale.x = 1;
    g_mesh.scale.y = 1;
    g_mesh.scale.z = 1;

    vec3 up = {0, 1, 0};
    vec3 target = {0, 0, 1};
    Mat4 pitch = mat4_rotate_x(camera.pitch);
    camera.direction = to_vec3(multiply(pitch, to_vec4(target)));
    target = add(camera.position, camera.direction);
    view_matrix = look_at(camera.position, target, up);

    world_matrix = mat4_scale(g_mesh.scale);
    world_matrix = multiply(mat4_rotate_x(g_mesh.rotation.x), world_matrix);
    world_matrix = multiply(mat4_rotate_y(g_mesh.rotation.y), world_matrix);
    world_matrix = multiply(mat4_rotate_z(g_mesh.rotation.z), world_matrix);
    world_matrix = multiply(mat4_translate(g_mesh.translation), world_matrix);

    triangles_to_render.clear();

    for (unsigned int i = 0; i < g_mesh.faces.size(); i++)
    {
        face face = g_mesh.faces[i];
        vec3 face_verticies[3];
        face_verticies[0] = g_mesh.vertices[face.a];
        face_verticies[1] = g_mesh.vertices[face.b];
        face_verticies[2] = g_mesh.vertices[face.c];

        vec4 transformed_verticies[3];
        for (int j = 0; j < 3; j++)
        {
            vec4 transformed_vertex = to_vec4(face_verticies[j]);

            transformed_vertex = multiply(world_matrix, transformed_vertex);
            transformed_vertex = multiply(view_matrix, transformed_vertex);
            transformed_verticies[j] = transformed_vertex;
        }

        vec3 triangle_normalv = get_normalv(transformed_verticies);
        if (render_options.enable_back_face_culling)
        {
            vec3 origin = {0, 0, 0};
            vec3 camerav = subtract(origin, to_vec3(transformed_verticies[0]));
            f32 alignment = dot(triangle_normalv, camerav);
            if (alignment < 0.0)
            {
                continue;
            }
        }

        u32 color = 0xFFFFFFFF;
        vec3 color_direction = {.x = 0, .y = 1, .z = 0};
        f32 color_alignment = (1.0 + dot(triangle_normalv, color_direction)) / 2;
        color = light_apply_intensity(color, color_alignment);

        // Clipping
        Polygon polygon = create_polygon_from_triangle(
            to_vec3(transformed_verticies[0]),
            to_vec3(transformed_verticies[1]),
            to_vec3(transformed_verticies[2]),
            face.a_uv,
            face.b_uv,
            face.c_uv);

        clip_polygon(&polygon);

        triangle triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
        i32 num_triangles_after_clipping = 0;
        triangles_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);

        for (i32 t = 0; t < num_triangles_after_clipping; t++)
        {
            triangle triangle_after_clipping = triangles_after_clipping[t];
            vec4 projected_points[3];
            for (int j = 0; j < 3; j++)
            {
                projected_points[j] = mat4_mul_vec4_project(projection, triangle_after_clipping.points[j]);

                projected_points[j].x *= (window_width / 2.0);
                projected_points[j].y *= (window_height / 2.0);

                // Obj files have inverte y, thus we need to flip it.
                projected_points[j].y *= -1;

                projected_points[j].x += (window_width / 2);
                projected_points[j].y += (window_height / 2);
            }

            triangle triangle_to_render = {
                .points = {
                    {
                        projected_points[0].x,
                        projected_points[0].y,
                        projected_points[0].z,
                        projected_points[0].w,
                    },
                    {
                        projected_points[1].x,
                        projected_points[1].y,
                        projected_points[1].z,
                        projected_points[1].w,
                    },
                    {
                        projected_points[2].x,
                        projected_points[2].y,
                        projected_points[2].z,
                        projected_points[2].w,
                    },
                },
                .texcoords = {
                    {triangle_after_clipping.texcoords[0]},
                    {triangle_after_clipping.texcoords[1]},
                    {triangle_after_clipping.texcoords[2]},
                },
                .color = color,
            };

            triangles_to_render.push_back(triangle_to_render);
        }
    }
}

void render(void)
{
    draw_grid();

    int num_triangles = triangles_to_render.size();
    for (int i = 0; i < num_triangles; i++)
    {
        Tex2 *tex = triangles_to_render[i].texcoords;
        vec4 *points = triangles_to_render[i].points;
        u32 color = triangles_to_render[i].color;
        if (render_options.draw_options.fill)
        {
            draw_filled_triangle(
                points[0].x, points[0].y, points[0].z, points[0].w,
                points[1].x, points[1].y, points[1].z, points[1].w,
                points[2].x, points[2].y, points[2].z, points[2].w,
                color);
        }

        if (render_options.draw_options.texture)
        {
            draw_textured_triangle(
                points[0].x, points[0].y, points[0].z, points[0].w, tex[0].u, tex[0].v,
                points[1].x, points[1].y, points[1].z, points[1].w, tex[1].u, tex[1].v,
                points[2].x, points[2].y, points[2].z, points[2].w, tex[2].u, tex[2].v,
                mesh_texture);
        }

        if (render_options.draw_options.wireframe)
        {
            draw_triangle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, 0xFFfcc729);
        }
        if (render_options.draw_options.dot)
        {
            draw_rect(points[0].x - 2.5, points[0].y - 2.5, 5, 5, 0xFFFF0000);
            draw_rect(points[1].x - 2.5, points[1].y - 2.5, 5, 5, 0xFFFF0000);
            draw_rect(points[2].x - 2.5, points[2].y - 2.5, 5, 5, 0xFFFF0000);
        }
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);
    clear_z_buffer();
    SDL_RenderPresent(renderer);
}

int main(void)
{
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        i32 time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
        if (time_to_wait < 0)
        {
            fprintf(stdout, "Misssed frame target by %d ms.\n", time_to_wait);
        }
        else if (time_to_wait <= FRAME_TARGET_TIME)
        {
            SDL_Delay(time_to_wait);
        }
        f32 dt = (SDL_GetTicks() - previous_frame_time) / 1000.0;
        previous_frame_time = SDL_GetTicks();
        process_input(dt);
        update(dt);
        render();
    }

    destroy_window();
    tear_down();

    return 0;
}