#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "math.h"

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 forward_velocity;
    f32 yaw;
    f32 pitch;
} Camera;

extern Camera camera;


#endif