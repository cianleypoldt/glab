#version 460 core

layout(location = 0) in vec3 aPos;

out vec3 out_color;

uniform vec2 window_dimensions;
uniform vec3 color;
uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    out_color = color;
}
