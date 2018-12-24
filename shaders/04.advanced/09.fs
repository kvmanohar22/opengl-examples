#version 330 core
out vec4 frag_col;

in vec3 col;

void main() {
  frag_col = vec4(col, 1.0f);
}
