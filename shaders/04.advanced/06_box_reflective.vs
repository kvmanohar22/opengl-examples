#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 inormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 position;

void main() {
   gl_Position = projection * view * model * vec4(ipos, 1.0f);
   normal = mat3(transpose(inverse(model))) * inormal;
   position = vec3(model * vec4(ipos, 1.0f));
}