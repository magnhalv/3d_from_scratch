
#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"
#include "vector.h"

typedef struct {
    f32 m[4][4];
} Mat4;

Mat4 mat4_identity();
Mat4 mat4_scale(f32 sx, f32 sy, f32 sz);
Mat4 mat4_translate(vec3 v);
Mat4 mat4_translate(f32 dx, f32 dy, f32 dz);
Mat4 mat4_scale(vec3 v);
Mat4 mat4_rotate_x(f32 angle);
Mat4 mat4_rotate_y(f32 angle);
Mat4 mat4_rotate_z(f32 angle);

Mat4 mat4_make_perspective(f32 fov, f32 aspect, f32 z_near, f32 z_far);
vec4 mat4_mul_vec4_project(Mat4 proj, vec4 v);
Mat4 look_at(vec3 eye, vec3 target, vec3 up);

vec4 multiply(Mat4 m, vec4 v);
Mat4 multiply(Mat4 a, Mat4 b);


#endif