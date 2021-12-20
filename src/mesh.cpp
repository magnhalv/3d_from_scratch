#include <fstream>
#include <iostream>
#include <ctype.h>

#include "mesh.h"

vec3 parse_vec_line(const char *line, i32 length);

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

    std::fstream stream;

    stream.open(file_name, std::ifstream::in);
    for (std::string line; std::getline(stream, line);) {
        if (line[0] == 'v' && line[1] == ' ') {
            vec3 vec = parse_vec_line(line.c_str(), line.size());
            std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;

        }
        if (line[0] == 'f' && line[1] == ' ') {
            //std::cout << line << std::endl;
        }
        
    }

    return result;
}

vec3 parse_vec_line(const char *line, i32 length) {
    f32 values[3];
    i32 index = 0;
    const char *start = nullptr;
    const char *end = nullptr;    
    bool digit_found = false;    
    for (int i = 0; i < length; i++) {
        if (start && (line[i] == ' ' || i == length - 1)) {
            values[index] = strtod(start, (char**)&end);
            index++;
            start = nullptr;
            end = nullptr;
            continue;
        }

        if (!start && (isdigit(line[i]) || line[i] == '-')) {
            start = &line[i];
        }

        if (start) {
            end = &line[i];
        }
    }

    vec3 result = { .x = values[0], .y = values[1], .z = values[2]};
    return result;
}