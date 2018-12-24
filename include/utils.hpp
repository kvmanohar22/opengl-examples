#ifndef _CUSTOM_UTILS_HPP_
#define _CUSTOM_UTILS_HPP_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <stb_image.h>                

#include <shader.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <camera.hpp>

namespace utils {

/**************************** LOAD TEXTURE FROM FILE ****************************/
unsigned int texture_from_file(
   const char *path) {
   
   std::string file_name = std::string(path);
   
   unsigned int texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, n_channels;      
   unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &n_channels, 0);
   if (data) {
      GLenum format;
      if (n_channels == 1)
         format = GL_RED;
      else if (n_channels == 3)
         format = GL_RGB;
      else if (n_channels == 4)
         format = GL_RGBA;

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   } else {
      std::cout << "Couldn't load the texture image with path: "
                << path
                << std::endl;
   }
   stbi_image_free(data);
  
   return texture_id;
}

/**************************** LOAD SKYBOX FROM FILE ****************************/
unsigned int load_cubemap(std::vector<std::string> faces) {
   unsigned int texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, n_channels;
   for (size_t i = 0; i < faces.size(); ++i) {
      unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &n_channels, 0);
      if (data) {
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                  );
         stbi_image_free(data);
      } else {
         std::cout << "Couldn't load the texture cube map path: "
                 << faces[i]
                 << std::endl;
      }
   }

   return texture_id;
}

/**************************** INPUT PROCESSING ****************************/
void process_input(GLFWwindow *window, float &last_frame, float &delta_time,
                   Camera &camera) {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    float camera_speed = 2.5f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, delta_time);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, delta_time);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, delta_time);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, delta_time);
}

} // namespace utils

#endif
