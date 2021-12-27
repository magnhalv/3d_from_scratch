#include "triangle.h"
#include <math.h>

void swap(i32 *a, i32 *b)
{
    i32 temp = *a;
    *a = *b;
    *b = temp;
}

/*
        (x0,y0)
          / \
         /   \
        /     \
       /       \
      /         \
  (x1,y1)------(x2,y2)
*/
void fill_flat_bottom_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color){
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
