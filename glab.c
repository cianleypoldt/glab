#include "glab.h"

#include <assert.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

glab_context ctx;

typedef struct {
        void* buffer;
        long  size;
} file;

file load_file(const char* path);
void free_file(file);

#define VS_PATH "shaders/vs_basic.glsl"
#define FS_PATH "shaders/fs_basic.glsl"

void resize_callback(GLFWwindow* window, int w, int h) {
        ctx.frame_width  = w;
        ctx.frame_height = h;
        if (ctx.on_resize) {
                ctx.on_resize();
        }
}

void glab_init_window(u64 width, u64 height, const char* title) {
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
        glEnable(GL_DEPTH_TEST);
}

void draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(ctx.window);
}

void glab_quit() {
        glfwDestroyWindow(ctx.window);
        glfwTerminate();
}

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

int should_close() {
        glfwPollEvents();
        return glfwWindowShouldClose(window_ptr);
}

uint32_t compile_shader_from_path(const char* path, shader_type type) {
        file src_file = load_file(path);
        if (src_file.size < 1) {
                printf("Error loading shader from file \"");
                printf("%s", path);
                printf("\"\n");
                free_file(src_file);
                return 0;
        }

        GLuint shader = 0;
        if (type == VERTEX) {
                shader = glCreateShader(GL_VERTEX_SHADER);
        } else if (type == FRAGMENT) {
                shader = glCreateShader(GL_FRAGMENT_SHADER);
        } else {
                return 0;
        }

        const char* srcv[] = { src_file.buffer };
        GLint length[]     = { (GLint) src_file.size };  // no null termination
        glShaderSource(shader, 1, srcv, length);
        free_file(src_file);

        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_TRUE) {
                return shader;
        }

        int len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* log = malloc(len + 1);
        glGetShaderInfoLog(shader, len, NULL, log);
        log[len] = '\n';
        printf("Shader loaded from \"");
        printf("%s", path);
        printf("\" failed to compile:\n    ");
        printf("%s\n", log);
        free(log);
        return 0;
}

file load_file(const char* path) {
        FILE* fp = fopen(path, "rb");
        if (!fp) {
                return (file) { NULL, 0 };
        }

        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (size < 0) {
                fclose(fp);
                return (file) { NULL, 0 };
        }
        void* buffer = malloc(size);
        if (!buffer) {
                fclose(fp);
                return (file) { NULL, 0 };
        }

        fread(buffer, 1, size, fp);
        fclose(fp);

        return (file) { buffer, size };
}

void free_file(file f) {
        free(f.buffer);
}
