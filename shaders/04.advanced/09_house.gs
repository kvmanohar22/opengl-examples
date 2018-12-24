#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
  vec3 col;
} gs_in[];
out vec3 col;

void main() {
  vec4 position = gl_in[0].gl_Position;
  col = gs_in[0].col;

  gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
  EmitVertex();
  gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);
  EmitVertex();
  gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);
  EmitVertex();
  gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);
  EmitVertex();
  col = vec3(1.0f);
  gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);
  EmitVertex();

  EndPrimitive();
}
