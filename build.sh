#!/usr/bin/env bash
set -e
( cd libmath && ./build.sh )
gcc main.c  libmath/libmath.a external/glad.c -lglfw -lm -o glab
