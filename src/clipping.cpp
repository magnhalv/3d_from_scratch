#include "clipping.h"

#define NUM_PLANES 6
Plane frustum_planes[NUM_PLANES];

void init_frustum_planes(f32 fov_x, f32 fov_y, f32 z_near, f32 z_far)
{
    f32 cos_half_fov_y = cos(fov_y / 2);
    f32 sin_half_fov_y = sin(fov_y / 2);

    f32 cos_half_fov_x = cos(fov_x / 2);
    f32 sin_half_fov_x = sin(fov_x / 2);

    vec3 origin = {0, 0, 0};

    frustum_planes[LEFT_FRUSTUM_PLANE].point = origin;
    frustum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov_x;
    frustum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
    frustum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

    frustum_planes[RIGHT_FRUSTUM_PLANE].point = origin;
    frustum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov_x;
    frustum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
    frustum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

    frustum_planes[TOP_FRUSTUM_PLANE].point = origin;
    frustum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
    frustum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov_y;
    frustum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

    frustum_planes[BOTTOM_FRUSTUM_PLANE].point = origin;
    frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
    frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov_y;
    frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

    frustum_planes[NEAR_FRUSTUM_PLANE].point = {0, 0, z_near};
    frustum_planes[NEAR_FRUSTUM_PLANE].normal = {0, 0, 1};

    frustum_planes[FAR_FRUSTUM_PLANE].point = {0, 0, z_far};
    frustum_planes[FAR_FRUSTUM_PLANE].normal = {0, 0, -1};
}

Polygon create_polygon_from_triangle(vec3 a, vec3 b, vec3 c, Tex2 a_uv, Tex2 b_uv, Tex2 c_uv)
{
    Polygon p = {
        .vertices = {a, b, c},
        .texcoords = {a_uv, b_uv, c_uv},
        .num_vertices = 3};
    return p;
}

void triangles_from_polygon(Polygon *polygon, triangle out_triangles[], i32 *num_out_triangles)
{
    for (i32 i = 0; i < (polygon->num_vertices - 2); i++)
    {
        i32 index0 = 0;
        i32 index1 = i + 1;
        i32 index2 = i + 2;

        out_triangles[i].points[0] = to_vec4(polygon->vertices[index0]);
        out_triangles[i].points[1] = to_vec4(polygon->vertices[index1]);
        out_triangles[i].points[2] = to_vec4(polygon->vertices[index2]);

        out_triangles[i].texcoords[0] = polygon->texcoords[index0];
        out_triangles[i].texcoords[1] = polygon->texcoords[index1];
        out_triangles[i].texcoords[2] = polygon->texcoords[index2];
        
    }
    *num_out_triangles = polygon->num_vertices - 2;
}

void clip_polygon(Polygon *polygon)
{
    clip_polygon_against_plane(polygon, LEFT_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, RIGHT_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, TOP_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, BOTTOM_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, NEAR_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, FAR_FRUSTUM_PLANE);
}

f32 linear_interpolation(f32 a, f32 b, f32 t) {
    return a + (t*(b - a));
}

void clip_polygon_against_plane(Polygon *polygon, i32 plane_index)
{
    vec3 plane_point = frustum_planes[plane_index].point;
    vec3 plane_normal = frustum_planes[plane_index].normal;

    vec3 inside_vertices[MAX_NUM_POLY_VERTICES];
    Tex2 inside_texcoords[MAX_NUM_POLY_VERTICES];
    int num_inside_vertices = 0;

    vec3 *current_vertex = &polygon->vertices[0];
    Tex2 *current_texcoord = &polygon->texcoords[0];

    vec3 *previous_vertex = &polygon->vertices[polygon->num_vertices - 1];
    Tex2 *previous_texcoord = &polygon->texcoords[polygon->num_vertices - 1];

    f32 current_dot;
    f32 previous_dot = dot(subtract(*previous_vertex, plane_point), plane_normal);

    // for loop instead?
    while (current_vertex != &polygon->vertices[polygon->num_vertices])
    {
        current_dot = dot(subtract(*current_vertex, plane_point), plane_normal);

        if (current_dot * previous_dot < 0)
        {
            f32 t = previous_dot / (previous_dot - current_dot);
            vec3 intersection_point = clone(current_vertex);
            intersection_point = subtract(intersection_point, *previous_vertex);
            intersection_point = multiply(intersection_point, t);
            intersection_point = add(intersection_point, *previous_vertex);
            inside_vertices[num_inside_vertices] = clone(&intersection_point);
            
            Tex2 intersection_texcoord = {
                .u = linear_interpolation(previous_texcoord->u, current_texcoord->u, t),
                .v = linear_interpolation(previous_texcoord->v, current_texcoord->v, t),
            };
            inside_texcoords[num_inside_vertices] = intersection_texcoord;
            num_inside_vertices++;
        }

        if (current_dot > 0)
        {               
            inside_vertices[num_inside_vertices] = *current_vertex;
            inside_texcoords[num_inside_vertices] = *current_texcoord;
            num_inside_vertices++;
        }

        previous_dot = current_dot;
        previous_vertex = current_vertex;
        previous_texcoord = current_texcoord;
        current_vertex++;
        current_texcoord++;
    }
 
    polygon->num_vertices = num_inside_vertices;
    for (i32 i = 0; i < polygon->num_vertices; i++)
    {
        polygon->vertices[i] = inside_vertices[i];
        polygon->texcoords[i] = inside_texcoords[i];
    }
}