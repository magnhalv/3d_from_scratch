#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include "vector.h"
#include "display.h"

typedef struct {
    i32 a; 
    i32 b;
    i32 c;
} face;

typedef struct {
    vec2 points[3];
} triangle;

void draw_filled_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color);

#endif