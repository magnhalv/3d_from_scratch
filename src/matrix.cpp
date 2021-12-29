#include <math.h>
#include "matrix.h"

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

vec4 mat4_mul_vec4(Mat4 m, vec4 v)
{
    vec4 result = {
        .x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
        .y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
        .z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
        .w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
    };
    return result;
}

Mat4 mat4_mul_mat4(Mat4 a, Mat4 b) {
    Mat4 result = {};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += a.m[i][k]*b.m[k][j];
            }
        }
    }
    return result;
}