#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include "vector.h"
#include "display.h"

typedef struct {
    i32 a; 
    i32 b;
    i32 c;
    u32 color;
} face;

typedef struct {
    vec4 points[3];
    u32 color;
    f32 average_depth;
} triangle;

void draw_filled_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color);


#endif