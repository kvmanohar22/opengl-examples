#version 330 core

out vec4 frag_col;
in vec3 tex;
uniform samplerCube skybox;

void main() {
	frag_col = texture(skybox, tex);
}