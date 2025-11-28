#include "glab.h"

#include <stdio.h>

int main() {
        printf("nigger");
        glab_create_window(800, 600);

        float mesh[24] = {
                -1, -1, 0.5,   //0
                1,  -1, 0.5,   //1
                -1, 1,  0.5,   //2
                1,  1,  0.5,   //3
                -1, -1, -0.5,  //4
                1,  -1, -0.5,  //5
                -1, 1,  -0.5,  //6
                1,  1,  -0.5   //7
        };

        uint32_t indices[36] = { 2, 6, 7, 2, 3, 7,

                                 //Bottom
                                 0, 4, 5, 0, 1, 5,

                                 //Left
                                 0, 2, 6, 0, 4, 6,

                                 //Right
                                 1, 3, 7, 1, 5, 7,

                                 //Front
                                 0, 2, 3, 0, 1, 3,

                                 //Back
                                 4, 6, 7, 4, 5, 7 };

        float* transform =
                add_opaque_mesh(mesh, 8, indices, 36, (float[3]) { 1, 1, 1 });
        draw();
        return 0;
        glab_quit();
}
