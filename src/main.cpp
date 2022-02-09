#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "input.h"
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

typedef struct
{
    Mesh mesh; // should be shared
    bool is_active;
} Projectile;

#define MAX_NUM_PROJECTILES 4
typedef struct
{
    Projectile player_projectiles[MAX_NUM_PROJECTILES];
    f32 projectile_pause;
    i32 next_projectile;
} GameState;

GameState game_state = {};

i32 previous_frame_time = 0;
bool is_running;

Mat4 projection;
Mat4 view_matrix;

TankMesh tank;

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

    //g_mesh = load_obj_file("./assets/tank.obj");
    tank = load_tank_obj_file("./assets/tank.obj");

    for (i32 i = 0; i < MAX_NUM_PROJECTILES; i++)
    {
        game_state.player_projectiles[i].mesh = load_obj_file("./assets/cube.obj");
    }

    // load_cube_mesh();
    load_png_texture_data("./assets/f22.png");
}

void tear_down()
{
    free_texture();
}

void process_input(GameControllerInput *input, f32 dt)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYUP:
        {
            SDL_Keycode key = event.key.keysym.sym;
            // Movement
            if (key == SDLK_w)
            {
                input->move_up.ended_down = false;
            }
            if (key == SDLK_s)
            {
                input->move_down.ended_down = false;
            }

            if (key == SDLK_a)
            {
                input->move_left.ended_down = false;
            }
            if (key == SDLK_d)
            {
                input->move_right.ended_down = false;
            }

            if (key == SDLK_SPACE)
            {
                input->fire.ended_down = false;
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            SDL_Keycode key = event.key.keysym.sym;
            if (key == SDLK_ESCAPE)
            {
                is_running = false;
            }
            if (key == SDLK_1)
            {
                render_options.draw_options = {0};
                render_options.draw_options.dot = true;
                render_options.draw_options.wireframe = true;
            }
            if (key == SDLK_2)
            {
                render_options.draw_options = {0};
                render_options.draw_options.wireframe = true;
            }
            if (key == SDLK_3)
            {
                render_options.draw_options = {0};
                render_options.draw_options.fill = true;
            }
            if (key == SDLK_4)
            {
                render_options.draw_options = {0};
                render_options.draw_options.wireframe = true;
                render_options.draw_options.fill = true;
            }
            if (key == SDLK_5)
            {
                render_options.draw_options = {0};
                render_options.draw_options.texture = true;
            }
            if (key == SDLK_6)
            {
                render_options.draw_options = {0};
                render_options.draw_options.texture = true;
                render_options.draw_options.wireframe = true;
            }
            if (key == SDLK_b)
            {
                render_options.enable_back_face_culling = true;
            }
            if (key == SDLK_x)
            {
                render_options.enable_back_face_culling = false;
            }

            // Movement
            if (key == SDLK_w)
            {
                input->move_up.ended_down = true;
            }
            if (key == SDLK_s)
            {
                input->move_down.ended_down = true;
            }

            if (key == SDLK_a)
            {
                input->move_left.ended_down = true;
            }
            if (key == SDLK_d)
            {
                input->move_right.ended_down = true;
            }

            if (key == SDLK_SPACE)
            {
                input->fire.ended_down = true;
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

void process_mesh(Mesh *mesh, std::vector<triangle> *triangles_to_render)
{    
    Mat4 world_matrix = mat4_scale(mesh->scale);
    world_matrix = multiply(mat4_rotate_x(mesh->rotation.x), world_matrix);
    world_matrix = multiply(mat4_rotate_y(mesh->rotation.y), world_matrix);
    world_matrix = multiply(mat4_rotate_z(mesh->rotation.z), world_matrix);
    world_matrix = multiply(mat4_translate(mesh->translation), world_matrix);

    for (unsigned int i = 0; i < mesh->faces.size(); i++)
    {
        face face = mesh->faces[i];
        vec3 face_verticies[3];
        face_verticies[0] = mesh->vertices[face.a];
        face_verticies[1] = mesh->vertices[face.b];
        face_verticies[2] = mesh->vertices[face.c];

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

            triangles_to_render->push_back(triangle_to_render);
        }
    }
}

void update(GameControllerInput *input, f32 dt)
{
    triangles_to_render.clear();
    if (input->move_up.ended_down)
    {
        tank.translation.z -= 10 * dt;
    }
    if (input->move_down.ended_down)
    {
        tank.translation.z += 10 * dt;
    }
    if (input->move_left.ended_down)
    {
        tank.translation.x += 10 * dt;
        tank.rotation.z = -M_PI / 8.0;
    }
    if (input->move_right.ended_down)
    {
        tank.translation.x -= 10 * dt;
        tank.rotation.z = M_PI / 8.0;
    }

    if (!input->move_left.ended_down && !input->move_right.ended_down) {
        tank.rotation.z = 0;
    }

    if (input->fire.ended_down)
    {        
        if (game_state.next_projectile < 4)
        {
            if (game_state.projectile_pause <= 0)
            {
                game_state.player_projectiles[game_state.next_projectile].is_active = true;
                game_state.player_projectiles[game_state.next_projectile].mesh.translation.x = tank.translation.x;
                game_state.player_projectiles[game_state.next_projectile].mesh.translation.y = tank.translation.y;
                game_state.player_projectiles[game_state.next_projectile].mesh.translation.z = tank.translation.z;
                game_state.projectile_pause = 0.5;
                game_state.next_projectile++;
            }
        }
    }

    if (game_state.projectile_pause > 0)
    {
        game_state.projectile_pause -= 1 * dt;
    }

    for (int i = 0; i < MAX_NUM_PROJECTILES; i++)
    {
        Projectile *projectile = &game_state.player_projectiles[i];
        if (projectile->is_active)
        {
            Mesh *mesh = &projectile->mesh;
            mesh->scale.x = 0.05;
            mesh->scale.y = 0.05;
            mesh->scale.z = 0.05;
            mesh->rotation.x += 1.0 * dt;
            mesh->rotation.y += 1.0 * dt;
            mesh->translation.z -= 10 * dt;         
        }
    }

    // g_mesh.rotation.x += 0.6 * dt;
    tank.rotation.y = M_PI / 2;
    tank.meshes[1].rotation.y += 0.6 * dt;
    tank.meshes[2].rotation.y += 0.6 * dt;

    tank.scale.x = 1;
    tank.scale.y = 1;
    tank.scale.z = 1;

    vec3 up = {0, 1, 0};
    vec3 target = {0, 0, 1};
    Mat4 pitch = mat4_rotate_x(camera.pitch);
    camera.direction = to_vec3(multiply(pitch, to_vec4(target)));
    target = add(camera.position, camera.direction);
    view_matrix = look_at(camera.position, target, up);
    process_mesh(&tank.meshes[0], &triangles_to_render);
    process_mesh(&tank.meshes[1], &triangles_to_render);
    process_mesh(&tank.meshes[2], &triangles_to_render);

    for (int i = 0; i < MAX_NUM_PROJECTILES; i++)
    {
        Projectile *projectile = &game_state.player_projectiles[i];
        if (projectile->is_active)
        {
            process_mesh(&projectile->mesh, &triangles_to_render);
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
    GameControllerInput input = {0};

    while (is_running)
    {
        i32 time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
        if (time_to_wait < 0)
        {
            // fprintf(stdout, "Misssed frame target by %d ms.\n", time_to_wait);
        }
        else if (time_to_wait <= FRAME_TARGET_TIME)
        {
            SDL_Delay(time_to_wait);
        }
        f32 dt = (SDL_GetTicks() - previous_frame_time) / 1000.0; // in seconds
        previous_frame_time = SDL_GetTicks();
        process_input(&input, dt);
        update(&input, dt);
        render();
    }

    destroy_window();
    tear_down();

    return 0;
}