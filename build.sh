#!/usr/bin/env bash
set -e
( cd libmath && ./build.sh )
gcc main.c glab.c shader_util.c libmath/libmath.a external/glad.c -lglfw -lm -o glab && ./glab
