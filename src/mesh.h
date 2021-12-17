#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERITICES 8
extern vec3 mesh_vertices[N_MESH_VERITICES];

#define N_MESH_FACES (6*2) // 6 cube faces, 2 triangles per face

extern face mesh_faces[N_MESH_FACES];

#endif;