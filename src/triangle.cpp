#include <math.h>
#include <iostream>

#include "triangle.h"
#include "swap.h"

/*
        (x0,y0)
          / \
         /   \
        /     \
       /       \
      /         \
  (x1,y1)------(x2,y2)
*/
void fill_flat_bottom_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color)
{
    f32 step_left = (x1 - x0) / (f32)(y1 - y0);
    f32 step_right = (x2 - x0) / (f32)(y2 - y0);

    f32 x_start = x0;
    f32 x_end = x0;
    for (i32 y = y0; y <= y2; y++)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start += step_left;
        x_end += step_right;
    }
}

/*
  (x0,y0)------(x1,y1)
      \         /
       \       /
        \     /
         \   /
          \ /
        (x2,y2)
*/
void fill_flat_top_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color)
{
    f32 step_left = (x0 - x2) / (f32)(y2 - y0);
    f32 step_right = (x1 - x2) / (f32)(y2 - y1);

    f32 x_start = x2;
    f32 x_end = x2;
    for (i32 y = y2; y >= y0; y--)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start += step_left;
        x_end += step_right;
    }
}

void draw_filled_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color)
{
    // We need to sort our y-coords (y0 < y1 < y2)
    if (y0 > y1)
    {
        swap(&y0, &y1);
        swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        swap(&y1, &y2);
        swap(&x1, &x2);
    }
    if (y0 > y1)
    {
        swap(&y0, &y1);
        swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    {
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        i32 m_y = y1;
        i32 m_x = ((x2 - x0) * (y1 - y0)) / (y2 - y0) + x0;
        // TODO: Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, m_x, m_y, color);

        // TODO: Draw flat-top triangle
        fill_flat_top_triangle(x1, y1, m_x, m_y, x2, y2, color);
    }
}

vec3 barycentric_weights(vec2 a, vec2 b, vec2 c, vec2 p)
{
    vec2 ab = subtract(b, a);
    vec2 bc = subtract(c, b);
    vec2 ac = subtract(c, a);
    vec2 ap = subtract(p, a);
    vec2 bp = subtract(p, b);

    f32 area_triangle_abc = (ab.x * ac.y) - (ab.y * ac.x);

    f32 alpha = ((bc.x * bp.y) - (bc.y * bp.x)) / area_triangle_abc;
    f32 beta = ((ap.x * ac.y) - (ap.y * ac.x)) / area_triangle_abc;
    f32 gamma = 1.0 - alpha - beta;

    vec3 result = {alpha, beta, gamma};
    return result;
}



void draw_texel(
    i32 x, i32 y, u32 *texture,
    vec4 point_a, vec4 point_b, vec4 point_c,
    Tex2 a_uv, Tex2 b_uv, Tex2 c_uv
)
{
    vec2 p = {x, y};
    vec2 a = to_vec2(&point_a);
    vec2 b = to_vec2(&point_b);
    vec2 c = to_vec2(&point_c);

    vec3 weights = barycentric_weights(a, b, c, p);

    f32 alpha = weights.x;
    f32 beta = weights.y;
    f32 gamma = weights.z;

    f32 interpolated_u = ((a_uv.u / point_a.w) * alpha) + ((b_uv.u / point_b.w) * beta) + ((c_uv.u / point_c.w) * gamma);
    f32 interpolated_v = ((a_uv.v / point_a.w) * alpha) + ((b_uv.v / point_b.w) * beta) + ((c_uv.v / point_c.w) * gamma);

    f32 interpolated_reciprocal_w = ((1.0 / point_a.w) * alpha) + (( 1.0 / point_b.w) * beta) + ((1.0 / point_c.w) * gamma);

    interpolated_u /= interpolated_reciprocal_w;
    interpolated_v /= interpolated_reciprocal_w;

    i32 tex_x = abs((int)(interpolated_u * texture_width));
    i32 tex_y = abs((int)(interpolated_v * texture_height));

    if ((tex_x < 0 || tex_x > texture_width) || (tex_y < 0 || tex_y > texture_height)) {
        // TODO : Fix barycentric going sum > 1.0
        std::cout << "Out of range. tex_x: " << tex_x << ". tex_y: " << tex_y << std::endl;        
        return;
    }

    draw_pixel(x, y, texture[(tex_y * texture_width) + tex_x]);
}

void draw_textured_triangle(
    i32 x0, i32 y0, f32 z0, f32 w0, f32 u0, f32 v0,
    i32 x1, i32 y1, f32 z1, f32 w1, f32 u1, f32 v1,
    i32 x2, i32 y2, f32 z2, f32 w2, f32 u2, f32 v2,
    u32 *texture)
{
    if (y0 > y1)
    {        
        swap(&x0, &x1);
        swap(&y0, &y1);
        swap(&z0, &z1);
        swap(&w0, &w1);
        swap(&u0, &u1);
        swap(&v0, &v1);        
    }
    if (y1 > y2)
    {
        swap(&y1, &y2);
        swap(&x1, &x2);
        swap(&z1, &z2);
        swap(&w1, &w2);
        swap(&u1, &u2);
        swap(&v1, &v2);        
    }
    if (y0 > y1)
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
        swap(&z0, &z1);
        swap(&w0, &w1);
        swap(&u0, &u1);
        swap(&v0, &v1);
    }

    vec4 a = { x0, y0, z0, w0 };
    vec4 b = { x1, y1, z1, w1 };
    vec4 c = { x2, y2, z2, w2 };
    Tex2 a_uv = { u0, v0 };
    Tex2 b_uv = { u1, v1 };
    Tex2 c_uv = { u2, v2 };

    f32 inv_slope1 = 0;
    f32 inv_slope2 = 0;

    if ((y1 - y0) != 0)
    {
        inv_slope1 = (x1 - x0) / (f32)(y1 - y0);
    }

    if ((y2 - y0) != 0)
    {
        inv_slope2 = (x2 - x0) / (f32)(y2 - y0);
    }

    if ((y1 - y0) != 0)
    {
        for (i32 y = y0; y <= y1; y++)
        {
            i32 x_start = x1 + ((y - y1) * inv_slope1);
            i32 x_end = x0 + (y - y0) * inv_slope2;

            if (x_start > x_end)
            {
                swap(&x_start, &x_end);
            }

            for (i32 x = x_start; x < x_end; x++)
            {
                draw_texel(
                    x, y, texture,
                    a, b, c,
                    a_uv, b_uv, c_uv);
            }
        }
    }

    inv_slope1 = 0;
    inv_slope2 = 0;

    if ((y2 - y1) != 0)
    {
        inv_slope1 = (x2 - x1) / (f32)(y2 - y1);
    }

    if ((y2 - y0) != 0)
    {
        inv_slope2 = (x2 - x0) / (f32)(y2 - y0);
    }

    if ((y2 - y1) != 0)
    {
        for (i32 y = y1; y <= y2; y++)
        {
            i32 x_start = x1 + ((y - y1) * inv_slope1);
            i32 x_end = x0 + (y - y0) * inv_slope2;

            if (x_start > x_end)
            {
                swap(&x_start, &x_end);
            }

            for (i32 x = x_start; x < x_end; x++)
            {
                draw_texel(
                    x, y, texture,
                    a, b, c,
                    a_uv, b_uv, c_uv);
            }
        }
    }

    // fill_flat_bottom_triangle(x0, y0, x1, y1, m_x, m_y, color);
    // fill_flat_top_triangle(x1, y1, m_x, m_y, x2, y2, color);
}
