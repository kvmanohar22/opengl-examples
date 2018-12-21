#version 330 core

in vec2 tex_pos;

out vec4 frag_col;

uniform sampler2D texture_sampler;
const float dd = 1.0 / 300.0;

void main() {
   // invert the colors
   // frag_col = vec4(vec3(1.0 - texture(texture_sampler, tex_pos)), 1.0);

   // grayscale image
   // frag_col = texture(texture_sampler, tex_pos);
   // float avg = (frag_col.r + frag_col.g + frag_col.b) / 3.0;
   // frag_col = vec4(avg, avg, avg, 1.0);	

   // weighted grayscale image
   // frag_col = texture(texture_sampler, tex_pos);
   // float avg = frag_col.r * 0.2126f + frag_col.g * 0.7152 + frag_col.b * 0.0722;
   // frag_col = vec4(avg, avg, avg, 1.0);	

   // kernel functionality
   vec2 offsets[9] = vec2[](
      vec2(-dd, -dd), 
      vec2(0, dd), 
      vec2(dd, dd), 
      vec2(-dd, 0), 
      vec2(0, 0), 
      vec2(dd, 0), 
      vec2(-dd, -dd), 
      vec2(0, -dd), 
      vec2(dd, -dd)
   );

   // kernel for blurring
   float kernel_simple[9] = float[](
   -1, -1, -1, 
   -1,  9, -1, 
   -1, -1, -1
   );

   // kernel for blurring
   float kernel_blur[9] = float[](
   	1.0 / 16, 2.0 / 16, 1.0 / 16,
   	2.0 / 16, 4.0 / 16, 2.0 / 16,
   	1.0 / 16, 2.0 / 16, 1.0 / 16
   );

   float kernel_edge[9] = float[](
   1, 1, 1, 
   1, -8, 1, 
   1, 1, 1
   );

   vec3 sample_tex[9];
   for(int i = 0; i < 9; ++i)
   		sample_tex[i] = vec3(texture(texture_sampler, tex_pos + offsets[i]));

   	vec3 final_col = vec3(0.0f);
   	for(int i = 0; i < 9; ++i)
   		final_col += kernel_edge[i] * sample_tex[i];	

   	frag_col = vec4(final_col, 1.0f);
}
