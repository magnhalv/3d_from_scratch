#ifndef MESH_H
#define MESH_H

#include <vector>

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERITICES 8
#define N_CUBE_FACES (6*2) // 6 cube faces, 2 triangles per face

//extern vec3 mesh_vertices[N_CUBE_VERITICES];
//extern face mesh_faces[N_CUBE_FACES];


typedef struct {
    std::vector<vec3> vertices; 
    std::vector<face> faces; 
    vec3 rotation;
    vec3 scale;
    vec3 translation;
} Mesh;

#define N_TANK_MESHES 3
typedef struct {
    Mesh meshes[N_TANK_MESHES]; //body, head, cannon
    vec3 origin_translations[N_TANK_MESHES];
    vec3 rotation;
    vec3 scale;
    vec3 translation;
} TankMesh;

void load_cube_mesh(void);
Mesh load_obj_file(const char *file_name);
TankMesh load_tank_obj_file(const char *file_name);

//extern Mesh g_mesh;

#endif