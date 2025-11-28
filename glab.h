#ifndef GLAB_IMPLEMENTATION
#define GLAB_IMPLEMENTATION
#include "external/glad/glad.h"

#include <GLFW/glfw3.h>
#include <stdint.h>

typedef enum { VERTEX, FRAGMENT } shader_type;

typedef enum { OPAQUE, TRANSPARENT } mesh_type;

typedef struct {
        float X, Y, Z;
} fv3;

typedef struct {
        uint64_t index;

        float transform[16];
} Mesh;

void glab_create_window(int width, int height);
void glab_quit();

float* add_opaque_mesh(float*    mesh,
                       long      vertex_count,
                       uint32_t* indicies,
                       long      index_count,
                       float     color[3]);
void   draw();

int should_close();

typedef struct {
        void* buffer;
        long  size;
} file;

uint32_t compile_shader_from_path(const char* path, shader_type type);
file     load_file(const char* path);
void     free_file(file);

#endif
