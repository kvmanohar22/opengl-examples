#version 330 core

in vec3 normal;
in vec3 position;

out vec4 frag_col;

uniform vec3 cam_pos;
uniform samplerCube skybox;

void main() {
	vec3 I = normalize(position - cam_pos);
	vec3 R = reflect(I, normalize(normal));
	frag_col = vec4(texture(skybox, R).rgb, 1.0f);
}
