#version 330 core
out vec4 frag_color;

in vec3 ocol;

void main() {
    frag_color = vec4(ocol, 1.0f);
}
