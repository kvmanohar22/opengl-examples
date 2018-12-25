#version 330 core 
out vec4 frag_color;

in VS_OUT {
    vec2 tex;
} vs_out;

uniform sampler2D texture_diffuse;

void main() {
   frag_color = texture(texture_diffuse, vs_out.tex);
}
