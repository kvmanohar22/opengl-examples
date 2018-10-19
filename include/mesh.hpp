#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <shader.hpp>

struct vertex {
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 tex_pos;
};

struct texture {
   unsigned int id;
   std::string type;
   std::string path;
};

class Mesh {
public:
   std::vector<vertex> vertices;
   std::vector<texture> textures;
   std::vector<unsigned int> indices;
       
   Mesh(std::vector<vertex> vertices,
        std::vector<texture> textures,
        std::vector<unsigned int> indices
        ) {
      this->vertices = vertices;
      this->textures = textures;
      this->indices  = indices;

      this->setup_mesh();
   }

   void pprint(int idx) {
      std::cout << "Mesh #"
                << idx
                << " #vertices: "
                << vertices.size()
                << " #textures: "
                << textures.size()
                << " #indices: "
                << indices.size()
                << std::endl;
   }

   void draw(Shader shader) {
      shader.use();
      unsigned int n_diffuse = 1;
      unsigned int n_specular = 1;

      for (int i = 0; i < textures.size(); ++i) {
         glActiveTexture(GL_TEXTURE0 + i);

         std::string number;
         std::string name = textures[i].type;
         if (name == "texture_diffuse") {
            number = std::to_string(n_diffuse++);
         } else if (name == "texture_specular") {
            number = std::to_string(n_specular++);
         } else {
            std::cerr << "Couldn't recognize texture type"
                      << std::endl;
         }

         shader.seti((name + number).c_str(), i);
         glBindTexture(GL_TEXTURE_2D, textures[i].id);
      }

      if (n_diffuse > 1)
        shader.seti("texture_diffuse", 0);
      if (n_specular > 1)
        shader.seti("texture_specular", 1);
      else
        shader.seti("texture_specular", 0);

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
   }
private:
   unsigned int VAO, VBO, EBO;

   void setup_mesh() {
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);
      
      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, 
                   vertices.size() * sizeof(vertex), 
                   &vertices[0], 
                   GL_STATIC_DRAW
                   );

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                   indices.size() * sizeof(unsigned int), 
                   &indices[0], 
                   GL_STATIC_DRAW
                   );

      glVertexAttribPointer(0, 3, GL_FLOAT, 
         GL_FALSE, sizeof(vertex), (void *)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, 
         GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, normal));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, 
         GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, tex_pos));
      glEnableVertexAttribArray(2);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
}

};

#endif

