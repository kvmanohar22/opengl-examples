#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 inorm;
layout (location = 2) in vec2 itex_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT {
    vec3 normal;
} vs_out;

void main() {
   gl_Position = projection * view * model * vec4(ipos, 1.0f);
   vs_out.normal = normalize(vec3(projection * view * vec4(mat3(transpose(inverse(model))) * inorm, 1.0f)));
}
