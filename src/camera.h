#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 forward_velocity;
    f32 yaw;
} Camera;

extern Camera camera;


#endif