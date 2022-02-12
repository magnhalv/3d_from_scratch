#include "camera.h"

Camera camera = {
    .position = {0.0, 0.0, -5.0},
    .direction = {0.0, 0.0, 0.0},
    .forward_velocity = {0.0, 0.0, 0.0},
    .yaw = 0,
    .pitch = 0};

Camera dev_camera = {};

void update_camera(Camera *camera_out, GameControllerInput *input, f32 dt)
{    
    vec3 target = {0, 0, 1};
    Mat4 camera_rotation = mat4_rotate_x(camera_out->pitch);
    camera_rotation = multiply(mat4_rotate_y(camera_out->yaw), camera_rotation);
    camera_out->direction = to_vec3(multiply(camera_rotation, to_vec4(target)));
    if (input->move_up.ended_down)
    {
        camera_out->position = add(camera_out->position, multiply(camera_out->direction, dt * 3));
    }

    if (input->move_down.ended_down)
    {
        camera_out->position = subtract(camera_out->position, multiply(camera_out->direction, dt * 3));
    }

    if (input->move_left.ended_down)
    {
        vec3 left_move = {-1, 0, 0};
        left_move = to_vec3(multiply(camera_rotation, to_vec4(left_move)));
        camera_out->position = add(camera_out->position, multiply(left_move, dt * 3));
    }

    if (input->move_right.ended_down)
    {
        vec3 right_move = {1, 0, 0};
        right_move = to_vec3(multiply(camera_rotation, to_vec4(right_move)));
        camera_out->position = add(camera_out->position, multiply(right_move, dt * 3));
    }
}

Mat4 calculate_view_matrix(Camera *camera, GameControllerInput *input)
{
    vec3 up = {0, 1, 0};    
    vec3 target = add(camera->position, camera->direction);
    Mat4 view_matrix = look_at(camera->position, target, up);
    return view_matrix;
}
