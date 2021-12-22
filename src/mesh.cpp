#include <fstream>
#include <iostream>
#include <ctype.h>

#include "mesh.h"

vec3 parse_vec_line(const char *line, i32 length);
face parse_face_line(const char *line, i32 length);

mesh g_mesh = {};

vec3 mesh_vertices[N_CUBE_VERITICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 0
    { .x = -1, .y =  1, .z = -1 }, // 1
    { .x =  1, .y =  1, .z = -1 }, // 2    
    { .x =  1, .y = -1, .z = -1 }, // 3
    { .x =  1, .y =  1, .z =  1 }, // 4 
    { .x =  1, .y = -1, .z =  1 }, // 5
    { .x = -1, .y =  1, .z =  1 }, // 6
    { .x = -1, .y = -1, .z =  1 }  // 7
};

face mesh_faces[N_CUBE_FACES] = {
    // front
    { .a = 0, .b = 1, .c = 2 },
    { .a = 0, .b = 2, .c = 3 },

    // right
    { .a = 3, .b = 2, .c = 4 },
    { .a = 3, .b = 4, .c = 5 },

    // back
    { .a = 5, .b = 4, .c = 6 },
    { .a = 5, .b = 6, .c = 7 },

    // left
    { .a = 7, .b = 6, .c = 1 },
    { .a = 7, .b = 1, .c = 0 },

    // top
    { .a = 1, .b = 6, .c = 4 },
    { .a = 1, .b = 4, .c = 2 },

    // bottom
    { .a = 5, .b = 7, .c = 0 },
    { .a = 5, .b = 0, .c = 3 },
};

void load_cube_mesh(void) { 
    for (int i = 0; i < N_CUBE_VERITICES; i++) {
        g_mesh.vertices.push_back(mesh_vertices[i]);
    }
    
    for (int i = 0; i < N_CUBE_FACES; i++) {
        g_mesh.faces.push_back(mesh_faces[i]);
    }
}

mesh load_obj_file(const char *file_name) {
    mesh result = {};

    
    FILE *file;
    file = fopen(file_name, "r");
    char line[1024];
    while (fgets(line, 1024, file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vec3 vec;
            sscanf(line, "v %f %f %f", &vec.x, &vec.y, &vec.z);
            result.vertices.push_back(vec);

        }
        if (line[0] == 'f' && line[1] == ' ') {            
            i32 vertex_indicies[3];
            i32 texture_indices[3];
            i32 normal_indices[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                &vertex_indicies[0], &texture_indices[0], &normal_indices[0],
                &vertex_indicies[1], &texture_indices[1], &normal_indices[1],
                &vertex_indicies[2], &texture_indices[2], &normal_indices[2]
            );            
            face face = { .a = vertex_indicies[0] - 1, .b = vertex_indicies[1] - 1, .c = vertex_indicies[2] - 1};
            result.faces.push_back(face);
        }
    }

    return result;
}