#include "vector.h"
#include <math.h>

/////////////
// 2D
/////////////

f32 magnitude(vec2 v) {
    return sqrt((v.x*v.x) + (v.y * v.y));
}

vec2 subtract(vec2 lhs, vec2 rhs) {
    vec2 result = {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y        
    };
    return result;
}
vec2 add(vec2 lhs, vec2 rhs) {
    vec2 result = {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y        
    };
    return result;
}

vec2 multiply(vec2 v, f32 factor) {
    vec2 result = {
        .x = v.x * factor,
        .y = v.y * factor
    };
    return result;
}
vec2 divide(vec2 v, f32 factor) {
    vec2 result = {
        .x = v.x / factor,
        .y = v.y / factor
    };
    return result;
}

f32 dot(vec2 a, vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

void normalize(vec2 *v) {
    f32 mag = magnitude(*v);
    v->x /= mag;
    v->y /= mag;
}

/////////////
// 3D
/////////////

vec3 rotate_x(vec3 v, f32 angle) {
    vec3 result = { 
        .x = v.x,
        .y = v.y*cos(angle) - v.z*sin(angle),
        .z = v.y*sin(angle) + v.z*cos(angle)
    };
    return result;
}
vec3 rotate_y(vec3 v, f32 angle) {
    vec3 result = { 
        .x = v.x*cos(angle) - v.z*sin(angle),
        .y = v.y,
        .z = v.x*sin(angle) + v.z*cos(angle)
    };
    return result;

}
vec3 rotate_z(vec3 v, f32 angle) {
    vec3 result = { 
        .x = v.x*cos(angle) - v.y*sin(angle),
        .y = v.x*sin(angle) + v.y*cos(angle),
        .z = v.z
    };
    return result;
}

f32 magnitude(vec3 v) {
    return sqrt((v.x*v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3 subtract(vec3 lhs, vec3 rhs) {
    vec3 result = {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
        .z = lhs.z - rhs.z        
    };
    return result;
}

vec3 add(vec3 lhs, vec3 rhs) {
    vec3 result = {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z        
    };
    return result;
}

vec3 multiply(vec3 v, f32 factor) {
    vec3 result = {
        .x = v.x * factor,
        .y = v.y * factor,
        .z = v.z * factor
    };
    return result;
}

vec3 divide(vec3 v, f32 factor) {
    vec3 result = {
        .x = v.x / factor,
        .y = v.y / factor,
        .z = v.z / factor
    };
    return result;
}

vec3 cross(vec3 a, vec3 b) {
    vec3 result = {
        .x = (a.y * b.z) - (a.z * b.y),
        .y = (a.z * b.x) - (a.x * b.z),
        .z = (a.x * b.y) - (a.y * b.x),
    };
    return result;
}

f32 dot(vec3 a, vec3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void normalize(vec3 *v) {
    f32 mag = magnitude(*v);
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
}

