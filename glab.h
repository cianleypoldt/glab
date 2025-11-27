#ifndef GLAB_IMPLEMENTATION
#define GLAB_IMPLEMENTATION
#include "external/glad/glad.h"
#include "libmath/linalg.h"

#include <GLFW/glfw3.h>

typedef enum { VERTEX, FRAGMENT } shader_type;

typedef struct {
        GLFWwindow* window;
        f32         frame_width, frame_height;
        void (*on_resize)();
} glab_context;

void glab_init();
void glab_quit();
void draw();

#endif
