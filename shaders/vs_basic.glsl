#version 330 core

layout(location = 0) in vec3 aPos;

uniform vec2 window_dimensions;
uniform vec3 color;
uniform mat4 transform;

out vec3 out_color;

void main(){
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    out_color = color;
};
