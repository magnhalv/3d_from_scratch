#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"

typedef struct {
    f32 u;
    f32 v;
} Tex2;

extern i32 texture_width;
extern i32 texture_height;

extern u32 *mesh_texture;
extern const u8 REDBRICK_TEXTURE[];

#endif