#include "texture.h"
#include <stdio.h>

i32 texture_width = 64;
i32 texture_height = 64;

upng_t *png_texture = nullptr;
u32 *mesh_texture = nullptr;

void load_png_texture_data(const char *filename) {
    png_texture = upng_new_from_file(filename);
    if (png_texture != nullptr) {
        upng_decode(png_texture);
        if (upng_get_error(png_texture) == UPNG_EOK) {
            mesh_texture = (u32*)upng_get_buffer(png_texture);
            texture_width = upng_get_width(png_texture);
            texture_height = upng_get_height(png_texture);
        }
        else {
            printf("Error1\n");
        }
    }
    else {
        printf("Error2\n");
    }
}

void free_texture() {
    upng_free(png_texture);
}

Tex2 clone(Tex2 *t) {
    Tex2 result = { t->u, t->v};
    return result;
}