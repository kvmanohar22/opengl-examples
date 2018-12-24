#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform float time;

void draw_line(vec4 position, vec3 normal) {
    gl_Position = position;
    EmitVertex();

    gl_Position = position + 0.1f * vec4(normalize(normal), 0.0f);    
    EmitVertex();

    EndPrimitive();
}

void main() {
    for (int i = 0; i < 3; ++i)
        draw_line(gl_in[i].gl_Position, gs_in[i].normal);
}
