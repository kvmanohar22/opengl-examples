#version 330 core
out vec4 frag_col;

in vec3 ocol;

void main() {
    frag_col = vec4(ocol, 1.0f);
}