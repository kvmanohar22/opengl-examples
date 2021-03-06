#version 330 core 
out vec4 frag_color;

in vec2 texf;

uniform sampler2D texture_diffuse;

void main() {
   frag_color = texture(texture_diffuse, texf);
}
