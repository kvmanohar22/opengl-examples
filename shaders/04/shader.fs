#version 330 core

in vec2 tex_pos;
out vec4 FragColor;
uniform sampler2D texture_sampler;

void main() {
   // Use this for normal rendering
   FragColor = texture(texture_sampler, tex_pos);
   
   // Use this to analyze z-buffer
   // FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
}
