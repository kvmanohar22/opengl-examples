#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itex_pos;

out vec2 tex_pos;

void main() {
   gl_Position = vec4(ipos.x, ipos.y, 0.0f, 1.0f);
   tex_pos = itex_pos;
}