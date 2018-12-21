#version 330 core
layout (location = 0) in vec3 ipos;

out vec3 tex;

uniform mat4 projection;
uniform mat4 view;

void main() {
	tex = ipos;
	vec4 pos = projection * view * vec4(ipos, 1.0f);
	gl_Position = pos.xyww;
}