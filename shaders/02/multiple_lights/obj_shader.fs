#version 330 core

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
struct Material {
 sampler2D diffuse;
 sampler2D specular;
 float shininess;
};

#define N_POINT_SOURCES 4
uniform point_source _point_sources[N_POINT_SOURCES];
uniform directional_source _dir_source;
uniform vec3 view_pos;
uniform Material material;

in vec3 frag_pos;
in vec3 norm;
in vec2 tex_pos;

out vec4 frag_color;

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
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
   // combine results
   vec3 ambient = light.ambient * vec3(texture(material.diffuse,  tex_pos));
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,  tex_pos));
   vec3 specular = light.specular * spec * vec3(texture(material.specular,  tex_pos));
   
   return (ambient + diffuse + specular);
}

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
   float spec = pow(max(dot(reflect_dir, view_dir), 0.0f), material.shininess);
   // attenuation
   float distance = length(frag_pos - light.position);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
   // combine results
   vec3 ambient = light.ambient * vec3(texture(material.diffuse,  tex_pos));
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,  tex_pos));
   vec3 specular = light.specular * spec * vec3(texture(material.specular,  tex_pos));
   
   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;

   return  (ambient + diffuse + specular);   
}

void main() {
   vec3 n_norm = normalize(norm);
   vec3 view_dir = normalize(view_pos - frag_pos);
   
   // Directional lighting
   vec3 result = calculate_directional_light(_dir_source, n_norm, view_dir);
   // Point lighting
   for (int i = 0; i < N_POINT_SOURCES; ++i) {
      result += calculate_point_light(_point_sources[i], n_norm, frag_pos, view_dir);
   }
   frag_color = vec4(result, 1.0f);
}

