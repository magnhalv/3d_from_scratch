#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} vec2;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} vec3;

vec3 rotate_x(vec3 v, f32 angle);
vec3 rotate_y(vec3 v, f32 angle);
vec3 rotate_z(vec3 v, f32 angle);

#endif