#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec2 tex;

in VS_OUT {
    vec2 tex;
} gs_in[];

uniform float time;

vec3 get_normal(vec4 pt0, vec4 pt1, vec4 pt2) {
    vec3 a = vec3(pt0) - vec3(pt1);
    vec3 b = vec3(pt2) - vec3(pt1);
    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2.0f;
    vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
    return position + vec4(direction, 0.0f);
}

void main() {
    vec4 pt0 = gl_in[0].gl_Position;
    vec4 pt1 = gl_in[1].gl_Position;
    vec4 pt2 = gl_in[2].gl_Position;

    vec3 normal = get_normal(pt0, pt1, pt2);

    gl_Position = explode(pt0, normal);
    tex = gs_in[0].tex;
    EmitVertex();

    gl_Position = explode(pt1, normal);
    tex = gs_in[1].tex;
    EmitVertex();

    gl_Position = explode(pt2, normal);
    tex = gs_in[2].tex;
    EmitVertex();

    EndPrimitive();
}
