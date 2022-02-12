#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "math.h"
#include "input.h"

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 forward_velocity;
    f32 yaw;
    f32 pitch;
} Camera;

void update_camera(Camera *camera_out, GameControllerInput *input, f32 dt);
Mat4 calculate_view_matrix(Camera *camera, GameControllerInput *input);

extern Camera camera;


#endif