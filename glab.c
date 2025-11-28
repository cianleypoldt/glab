#include "glab.h"

#define LIBMATH_SINGLE_PRECISION
#include "libmath/linalg.h"

#include <assert.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
        GLFWwindow* window;
        int         frame_width, frame_height;
        void (*on_resize)();
} ctx;

#define VS_PATH "shaders/vs_basic.glsl"
#define FS_PATH "shaders/fs_basic.glsl"

void resize_callback(GLFWwindow* window, int w, int h) {
        ctx.frame_width  = w;
        ctx.frame_height = h;
        if (ctx.on_resize) {
                ctx.on_resize();
        }
}

void glab_init_window(int width, int height) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        ctx.window = glfwCreateWindow(1, 1, "glab", NULL, NULL);
        if (ctx.window == NULL) {
                glfwTerminate();
                return;
        }
        glfwMakeContextCurrent(ctx.window);
        glfwSetFramebufferSizeCallback(ctx.window, resize_callback);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                fprintf(stderr, "Failed to initialize GLAD");
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void glab_quit() {
        glfwDestroyWindow(ctx.window);
        glfwTerminate();
}

void draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(ctx.window);
}

/*
void init_resources() {
        glGenVertexArrays(1, &triangle_vao);
        glBindVertexArray(triangle_vao);

        GLuint cube_vbo = 0;
        glGenBuffers(1, &cube_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices),
                     triangle_vertices, GL_STATIC_DRAW);

        GLuint cube_ebo;
        glGenBuffers(1, &cube_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices),
                     cube_indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void*) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        GLuint vs_basic   = compile_shader_from_path(VS_PATH, VERTEX);
        GLuint fs_basic   = compile_shader_from_path(FS_PATH, FRAGMENT);
        basic_shader_prog = glCreateProgram();
        glAttachShader(basic_shader_prog, vs_basic);
        glAttachShader(basic_shader_prog, fs_basic);
        glLinkProgram(basic_shader_prog);
        glDeleteShader(vs_basic);
        glDeleteShader(fs_basic);
}

*/
int should_close() {
        glfwPollEvents();
        return glfwWindowShouldClose(ctx.window);
}
