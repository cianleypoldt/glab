#ifndef GLAB_IMPLEMENTATION
#define GLAB_IMPLEMENTATION
#include "external/glad/glad.h"

#include <GLFW/glfw3.h>

typedef enum { VERTEX, FRAGMENT } shader_type;

typedef struct {
        float X, Y, Z;
} fv3;

typedef struct {
} scene;

void glab_init_window(int width, int height);
void glab_quit();
void draw();

int should_close();

typedef struct {
        void* buffer;
        long  size;
} file;

uint32_t compile_shader_from_path(const char* path, shader_type type);
file     load_file(const char* path);
void     free_file(file);

#endif
