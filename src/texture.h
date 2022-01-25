#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"
#include "lib/upng.h"

typedef struct {
    f32 u;
    f32 v;
} Tex2;

extern i32 texture_width;
extern i32 texture_height;

extern upng_t* png_texture;
extern u32 *mesh_texture;
extern const u8 REDBRICK_TEXTURE[];


void load_png_texture_data(const char *filename);
void free_texture();
Tex2 clone(Tex2 *t);

#endif