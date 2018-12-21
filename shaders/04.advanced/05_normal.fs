#version 330 core

in vec2 tex_pos;
out vec4 frag_col;
uniform sampler2D texture_sampler;

void main() {
   frag_col = texture(texture_sampler, tex_pos);
}
