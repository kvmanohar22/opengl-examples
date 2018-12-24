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
   mat3 normalmatrix = mat3(transpose(inverse(view * model)));
   vs_out.normal = normalize(vec3(projection * vec4(normalmatrix * inorm, 0.0f)));
}
