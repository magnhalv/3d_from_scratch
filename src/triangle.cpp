#include <math.h>

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

void draw_textured_triangle(
    i32 x0, i32 y0, f32 u0, f32 v0,
    i32 x1, i32 y1, f32 u1, f32 v1,
    i32 x2, i32 y2, f32 u2, f32 v2,
    u32 *texture)
{
    if (y0 > y1)
    {
        swap(&y0, &y1);
        swap(&x0, &x1);
        swap(&u0, &u1);
        swap(&v0, &v1);
    }
    if (y1 > y2)
    {
        swap(&y1, &y2);
        swap(&x1, &x2);
        swap(&u1, &u2);
        swap(&v1, &v2);
    }
    if (y0 > y1)
    {
        swap(&y0, &y1);
        swap(&x0, &x1);
        swap(&u0, &u1);
        swap(&v0, &v1);
    }

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
                draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF : 0xFF000000);
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
                draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF : 0xFF000000);
            }
        }
    }

    // fill_flat_bottom_triangle(x0, y0, x1, y1, m_x, m_y, color);
    // fill_flat_top_triangle(x1, y1, m_x, m_y, x2, y2, color);
}
