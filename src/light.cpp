#include "light.h"

u32 light_apply_intensity(u32 original_color, f32 percentage) {
    u32 a = (original_color & 0xFF000000);
    u32 r = (original_color & 0x00FF0000) * percentage;
    u32 g = (original_color & 0x0000FF00) * percentage;
    u32 b = (original_color & 0x000000FF) * percentage;

    u32 result = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    return result;
}