#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"
#include "vector.h"

typedef struct {
    f32 m[4][4];
} Mat4;

Mat4 mat4_identity();
Mat4 mat4_scale(f32 sx, f32 sy, f32 sz);
Mat4 mat4_translate(f32 dx, f32 dy, f32 dz);
Mat4 mat4_scale(vec3 v);
Mat4 mat4_rotate_x(f32 angle);
Mat4 mat4_rotate_y(f32 angle);
Mat4 mat4_rotate_z(f32 angle);

vec4 mat4_mul_vec4(Mat4 m, vec4 v);
Mat4 mat4_mul_mat4(Mat4 a, Mat4 b);


#endif