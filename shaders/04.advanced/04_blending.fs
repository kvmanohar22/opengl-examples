#version 330 core

in vec2 tex_pos;
out vec4 frag_col;
uniform sampler2D texture_sampler;

void main() {
   vec4 temp_col = texture(texture_sampler, tex_pos);
   if (temp_col.a < 0.1)
   		discard;
   frag_col = temp_col;
}
