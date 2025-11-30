#include "glab.h"

#include <stdint.h>

#define LIBMATH_SINGLE_PRECISION
#include "libmath/linalg.h"

#include <assert.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPAQUE 1024

struct {
        long   m_count;
        GLuint vao[MAX_OPAQUE];
        GLuint vbo[MAX_OPAQUE];
        GLuint ebo[MAX_OPAQUE];
        long   index_count[MAX_OPAQUE];
        float* transform[MAX_OPAQUE];
        float  color[MAX_OPAQUE][3];
} opaque_mesh_data;

struct {
        GLFWwindow* window;
        int         frame_width, frame_height;

        GLint window_dimensions;

        GLuint opaque_prog;
        GLint  u_color;
        GLint  u_transform;

} ctx;

#define VS_PATH "shaders/vs_basic.glsl"
#define FS_PATH "shaders/fs_basic.glsl"

void resize_callback(GLFWwindow* window, int w, int h) {
        ctx.frame_width  = w;
        ctx.frame_height = h;
        glViewport(0, 0, w, h);
        glUniform2f(ctx.window_dimensions, (float) w, (float) h);
}

void glab_create_window(int width, int height) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        ctx.window = glfwCreateWindow(100, 100, "glab", NULL, NULL);
        if (ctx.window == NULL) {
                glfwTerminate();
                return;
        }
        glfwMakeContextCurrent(ctx.window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                fprintf(stderr, "Failed to initialize GLAD");
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        GLuint vs_shader = compile_shader_from_path(VS_PATH, VERTEX);
        GLuint fs_shader = compile_shader_from_path(FS_PATH, FRAGMENT);

        ctx.opaque_prog = glCreateProgram();
        glAttachShader(ctx.opaque_prog, vs_shader);
        glAttachShader(ctx.opaque_prog, fs_shader);
        glLinkProgram(ctx.opaque_prog);

        GLint linked = 0;
        glGetProgramiv(ctx.opaque_prog, GL_LINK_STATUS, &linked);
        if (!linked) {
                GLint len = 0;
                glGetProgramiv(ctx.opaque_prog, GL_INFO_LOG_LENGTH, &len);
                char* log = malloc(len ? len : 1);
                glGetProgramInfoLog(ctx.opaque_prog, len, NULL, log);
                fprintf(stderr, "Program link error: %s\n", log);
                free(log);
                glDeleteProgram(ctx.opaque_prog);
        }

        glDeleteShader(vs_shader);
        glDeleteShader(fs_shader);

        ctx.window_dimensions =
                glGetUniformLocation(ctx.opaque_prog, "window_dimensions");

        ctx.u_color     = glGetUniformLocation(ctx.opaque_prog, "color");
        ctx.u_transform = glGetUniformLocation(ctx.opaque_prog, "transform");

        printf("%i, %i, %i\n", ctx.window_dimensions, ctx.u_transform,
               ctx.u_color);
        memset(&opaque_mesh_data, 0, sizeof(opaque_mesh_data));
        glEnable(GL_DEPTH_TEST);
        glfwSetFramebufferSizeCallback(ctx.window, resize_callback);
}

void glab_quit() {
        glDeleteVertexArrays(opaque_mesh_data.m_count, opaque_mesh_data.vao);
        glDeleteBuffers(opaque_mesh_data.m_count, opaque_mesh_data.vbo);
        glDeleteBuffers(opaque_mesh_data.m_count, opaque_mesh_data.ebo);

        for (int i = 0; i < opaque_mesh_data.m_count; i++) {
                free(opaque_mesh_data.transform[i]);
        }

        glfwDestroyWindow(ctx.window);
        glfwTerminate();
}

void draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(ctx.opaque_prog);
        for (int i = 0; i < opaque_mesh_data.m_count; i++) {
                float* color = opaque_mesh_data.color[i];
                glUniform3f(ctx.u_color, color[0], color[1], color[2]);
                float* transform = opaque_mesh_data.transform[i];
                glUniformMatrix4fv(ctx.u_transform, 1, GL_FALSE, transform);

                glBindVertexArray(opaque_mesh_data.vao[i]);

                glDrawElements(GL_TRIANGLES, opaque_mesh_data.index_count[i],
                               GL_UNSIGNED_INT, NULL);
        }
        glfwSwapBuffers(ctx.window);
}

float* add_opaque_mesh(float*    mesh,
                       long      vertex_count,
                       uint32_t* indicies,
                       long      index_count,
                       float     color[3]) {
        if (opaque_mesh_data.m_count >= MAX_OPAQUE) {
                printf("Mesh limit exceeded.");
                return NULL;
        }

        GLuint* vao = &opaque_mesh_data.vao[opaque_mesh_data.m_count];
        GLuint* vbo = &opaque_mesh_data.vbo[opaque_mesh_data.m_count];
        GLuint* ebo = &opaque_mesh_data.ebo[opaque_mesh_data.m_count];
        float** transform =
                &opaque_mesh_data.transform[opaque_mesh_data.m_count];

        glGenVertexArrays(1, vao);
        glBindVertexArray(*vao);
        glGenBuffers(1, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float), mesh,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*) 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t),
                     indicies, GL_STATIC_DRAW);

        opaque_mesh_data.index_count[opaque_mesh_data.m_count] = index_count;
        memcpy(&opaque_mesh_data.color[opaque_mesh_data.m_count], color,
               3 * sizeof(float));
        *transform = malloc(16 * sizeof(float));
        if (*transform) {
                opaque_mesh_data.m_count++;
                return *transform;
        }
        printf("failed to make mesh\n");
        exit(1);
        return NULL;
}

int should_close() {
        glfwPollEvents();
        return glfwWindowShouldClose(ctx.window);
}
