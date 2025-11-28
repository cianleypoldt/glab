#ifndef GLAB_IMPLEMENTATION
#define GLAB_IMPLEMENTATION
#include "external/glad/glad.h"

#include <GLFW/glfw3.h>

typedef enum { VERTEX, FRAGMENT } shader_type;

typedef struct {
        float X, Y, Z;
} fv3;

typedef struct {
        GLFWwindow* window;
        int         frame_width, frame_height;
        void (*on_resize)();
} glab_context;

typedef struct {
} scene;

void glab_init_window(int width, int height);
void glab_quit();
void draw();

#endif
