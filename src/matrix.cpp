#include <math.h>
#include "matrix.h"

#include <stdio.h>

Mat4 mat4_identity()
{
    Mat4 result = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

Mat4 mat4_translate(vec3 v)
{
    Mat4 result = mat4_translate(v.x, v.y, v.z);
    return result;
}

Mat4 mat4_translate(f32 dx, f32 dy, f32 dz)
{
    Mat4 result = {{
        {1, 0, 0, dx},
        {0, 1, 0, dy},
        {0, 0, 1, dz},
        {0, 0, 0, 1},
    }};
    return result;
}

Mat4 mat4_rotate_x(f32 angle)
{
    Mat4 result = {{
        {1, 0, 0, 0},
        {0, cos(angle), -sin(angle), 0},
        {0, sin(angle), cos(angle), 0},
        {0, 0, 0, 1},
    }};
    return result;
}

Mat4 mat4_rotate_y(f32 angle)
{
    Mat4 result = {{
        {cos(angle), 0, sin(angle), 0},
        {0, 1, 0, 0},
        {-sin(angle), 0, cos(angle), 0},
        {0, 0, 0, 1},
    }};
    return result;
}

Mat4 mat4_rotate_z(f32 angle)
{
    Mat4 result = {{
        {cos(angle), -sin(angle), 0, 0},
        {sin(angle), cos(angle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

Mat4 mat4_scale(vec3 v)
{
    return mat4_scale(v.x, v.y, v.z);
}

Mat4 mat4_scale(f32 sx, f32 sy, f32 sz)
{
    Mat4 result = {{
        {sx, 0, 0, 0},
        {0, sy, 0, 0},
        {0, 0, sz, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

vec4 multiply(Mat4 m, vec4 v)
{
    vec4 result = {
        .x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
        .y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
        .z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
        .w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
    };
    return result;
}

Mat4 multiply(Mat4 a, Mat4 b) {
    Mat4 result = {{{0}}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0.0;
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += a.m[i][k]*b.m[k][j];
            }
        }
    }
    return result;
}

Mat4 mat4_make_perspective(f32 fov, f32 aspect, f32 z_near, f32 z_far) {
    Mat4 result = {{{0}}};
    f32 fov_calc = 1 / (tan(fov/2.0));
    result.m[0][0] = aspect*fov_calc;
    result.m[1][1] = fov_calc;
    result.m[2][2] = z_far / (z_far - z_near);
    result.m[2][3] = (-z_far*z_near) / (z_far - z_near);
    result.m[3][2] = 1.0;
    return result;
}

vec4 mat4_mul_vec4_project(Mat4 proj, vec4 v) {
    vec4 result = multiply(proj, v);

    if (result.w != 0) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }
    return result;
}

Mat4 look_at(vec3 eye, vec3 target, vec3 up) {
    vec3 z = normalize(subtract(target, eye));
    vec3 x = normalize(cross(up, z));
    vec3 y = cross(z, x);

    Mat4 view = {{
        x.x, x.y, x.z, -dot(x, eye),
        y.x, y.y, y.z, -dot(y, eye),
        z.x, z.y, z.z, -dot(z, eye),
        0,   0,   0,   -1 
    }};

    return view;
}
