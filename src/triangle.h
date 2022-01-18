#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include "vector.h"
#include "display.h"
#include "texture.h"

typedef struct
{
    i32 a;
    i32 b;
    i32 c;
    Tex2 a_uv;
    Tex2 b_uv;
    Tex2 c_uv;
    u32 color;
} face;

typedef struct
{
    vec4 points[3];
    Tex2 texcoords[3];
    u32 color;    
} triangle;

void draw_filled_triangle(    
    i32 x0, i32 y0, f32 z0, f32 w0,
    i32 x1, i32 y1, f32 z1, f32 w1,
    i32 x2, i32 y2, f32 z2, f32 w2,
    u32 color);
vec3 barycentric_weights(vec2 a, vec2 b, vec2 c, vec2 p);
void draw_textured_triangle(
    i32 x0, i32 y0, f32 z0, f32 w0, f32 u0, f32 v0,
    i32 x1, i32 y1, f32 z1, f32 w1, f32 u1, f32 v1,
    i32 x2, i32 y2, f32 z2, f32 w2, f32 u2, f32 v2,
    u32 *texture);
void draw_texel(
    i32 x, i32 y, u32 *texture,
    vec2 a, vec2 b, vec2 c,
    Tex2 a_uv, Tex2 b_uv, Tex2 c_uv);

#endif