#version 330 core
layout (location = 0) in vec2 ipos;
layout (location = 1) in vec3 icol;

out VS_OUT {
    vec3 col;
} vs_out;

void main() {
  gl_Position = vec4(ipos.x, ipos.y, 0.0f, 1.0f);
  vs_out.col = icol;
}
