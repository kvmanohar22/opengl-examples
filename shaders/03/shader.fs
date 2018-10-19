#version 330 core 
out vec4 frag_color;

struct directional_source {
   vec3 direction;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;  
};

struct point_source {
   vec3 position;
   
   float constant;
   float linear;
   float quadratic;
   
   vec3 ambient;
   vec3 diffuse;
   vec3 specular; 
};

in vec3 frag_pos;
in vec2 tex_pos;
in vec3 norm;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform directional_source _dir_source;
uniform point_source _point_source;
uniform vec3 view_pos;

vec3 calculate_point_light(
   point_source light,
   vec3 normal,
   vec3 frag_pos,
   vec3 view_dir
) {
   vec3 light_dir = normalize(light.position - frag_pos);
   // diffuse shading
   float diff = max(dot(normal, light_dir), 0.0f);
   // specular shading
   vec3 reflect_dir = reflect(-light_dir, normal);
   float spec = pow(max(dot(reflect_dir, view_dir), 0.0f), 64.0f);
   // attenuation
   float distance = length(frag_pos - light.position);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
   // combine results
   vec3 ambient = light.ambient * vec3(texture(texture_diffuse,  tex_pos));
   vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse,  tex_pos));
   vec3 specular = light.specular * spec * vec3(texture(texture_specular,  tex_pos));
   
   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;

   return  (ambient + diffuse + specular);   
}

vec3 calculate_directional_light(
   directional_source light,
   vec3 normal,
   vec3 view_dir
) {

   vec3 light_dir = normalize(-light.direction);
   // diffuse shading
   float diff = max(dot(normal, light_dir), 0.0f);
   // specular shading
   vec3 reflect_dir = reflect(-light_dir, normal);
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 64.0f);
   // combine results
   vec3 ambient = light.ambient * vec3(texture(texture_diffuse, tex_pos));
   vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, tex_pos));
   vec3 specular = light.specular * spec * vec3(texture(texture_specular, tex_pos));

   return (ambient + diffuse + specular);
}

void main() {
    // Some mistakes that were made
    // 1. Don't use the generalized transformation matrix for transforming the normals to world co-ordinates
    // 2. Scale the diffuse and specular ligthtings with `cos_theta` and `specularity` respectively

   vec3 n_norm = normalize(norm);
   vec3 view_dir = normalize(view_pos - frag_pos);

   // Directional lighting
   vec3 result = calculate_directional_light(_dir_source, n_norm, view_dir);
   result += calculate_point_light(_point_source, n_norm, frag_pos, view_dir);

   frag_color = vec4(result, 1.0f);
}
