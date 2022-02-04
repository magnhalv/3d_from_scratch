#ifndef CLIPPING_H
#define CLIPPING_H

#include "vector.h"
#include "triangle.h"
#include "types.h"
#include "math.h"

enum {
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE,
};

typedef struct {
    vec3 point;
    vec3 normal;
} Plane;

#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIANGLES MAX_NUM_POLY_VERTICES-2

typedef struct {
    vec3 vertices[MAX_NUM_POLY_VERTICES];
    Tex2 texcoords[MAX_NUM_POLY_VERTICES];
    i32 num_vertices;
} Polygon;

void init_frustum_planes(f32 fov_x, f32 fov_y, f32 z_near, f32 z_far);
Polygon create_polygon_from_triangle(vec3 a, vec3 b, vec3 c, Tex2 a_uv, Tex2 b_uv, Tex2 c_uv);
void triangles_from_polygon(Polygon *polygon, triangle out_triangles[], i32 *num_out_triangles);
void clip_polygon(Polygon *polygon);
void clip_polygon_against_plane(Polygon *polygon, i32 plane_index);

#endif