#include "glab.h"

#include <stdio.h>
#include <stdlib.h>

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
