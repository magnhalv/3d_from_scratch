#include "swap.h"

void swap(i32 *a, i32 *b)
{
    i32 temp = *a;
    *a = *b;
    *b = temp;
}

void swap(f32 *a, f32 *b)
{
    i32 temp = *a;
    *a = *b;
    *b = temp;
}