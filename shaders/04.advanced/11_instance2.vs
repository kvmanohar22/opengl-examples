#version 330 core

layout (location = 0) in vec2 ipos;
layout (location = 1) in vec3 icol;
layout (location = 2) in vec2 ioffset;

out vec3 ocol;

void main() {
    gl_Position = vec4(ipos + ioffset, 0.0f, 1.0f);
    ocol = icol;
}