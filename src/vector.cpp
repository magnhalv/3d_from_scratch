#include "vector.h"
#include <math.h>

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