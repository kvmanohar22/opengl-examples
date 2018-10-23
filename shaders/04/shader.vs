#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itex_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tex_pos;

void main() {
   gl_Position = projection * view * model * vec4(ipos, 1.0f);
   tex_pos = itex_pos;
}