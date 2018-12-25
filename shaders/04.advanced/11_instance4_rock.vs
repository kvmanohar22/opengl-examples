#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 inorm;
layout (location = 2) in vec2 itex_pos;
layout (location = 3) in mat4 trans_mat;

uniform mat4 projection;
uniform mat4 view;

out VS_OUT {
    vec2 tex;
} vs_out;

void main() {
   gl_Position = projection * view * trans_mat * vec4(ipos, 1.0f);
   vs_out.tex = itex_pos;
}
