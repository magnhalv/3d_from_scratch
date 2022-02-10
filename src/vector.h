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

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} vec4;

// 2D

f32 magnitude(vec2 v);
vec2 subtract(vec2 lhs, vec2 rhs);
vec2 add(vec2 lhs, vec2 rhs);
vec2 multiply(vec2 v, f32 factor);
vec2 divide(vec2 v, f32 factor);
f32 dot(vec2 v, f32 factor);
void normalize(vec2 *v);
vec2 to_vec2(vec4 *v);

// 3D

f32 magnitude(vec3 v);
vec3 rotate_x(vec3 v, f32 angle);
vec3 rotate_y(vec3 v, f32 angle);
vec3 rotate_z(vec3 v, f32 angle);
vec3 subtract(vec3 lhs, vec3 rhs);
vec3 add(vec3 lhs, vec3 rhs);
vec3 multiply(vec3 v, f32 factor);
vec3 divide(vec3 v, f32 factor);
vec3 cross(vec3 a, vec3 b);
f32 dot(vec3 a, vec3 b);
vec3 to_vec3(vec4 v);
void normalize(vec3 *v);
vec3 normalize(vec3 v);
vec3 clone(vec3 *v);
vec3 negative(vec3 *v);

// 4D

vec4 to_vec4(vec3 v);

#endif