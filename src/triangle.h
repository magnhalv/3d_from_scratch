#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include "vector.h"

typedef struct {
    i32 a; 
    i32 b;
    i32 c;
} face;

typedef struct {
    vec2 points[3];
} triangle;

#endif