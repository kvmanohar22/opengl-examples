/*
  0. 

*/

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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>                

#include <shader.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <camera.hpp>

// for adjusting camera speed
float delta_time = 0.0f;
float last_frame = 0.0f;

// callbacks
bool first_mouse = true;
double x_last = 400.0f;
double y_last = 300.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void mouse_callback(GLFWwindow *window, double x_new, double y_new) {
   if (first_mouse) {
      x_last = x_new;
      y_last = y_new;
      first_mouse = false;
   }
   float dx = x_new - x_last;
   float dy = y_last - y_new;
   x_last = x_new;
   y_last = y_new;
   
   camera.process_mouse_movement(dx, dy);
}

void process_input(GLFWwindow *window) {
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

void scroll_callback(GLFWwindow *window, double dx, double dy) {
  camera.process_scroll(dy);
}

unsigned int texture_from_file(
   const char *path) {
   
   std::string file_name = std::string(path);
   
   unsigned int texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
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

       if (format == GL_RGBA) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
       } else {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
       }


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

int main() {
   int s_width = 1600, s_height = 720;
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   GLFWwindow *window = glfwCreateWindow(s_width, s_height, "Depth Testing", NULL, NULL);
   if (window == NULL) {
      std::cout << "Couldn't create window!";
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(window);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   // Enable the depth buffer
   glEnable(GL_DEPTH_TEST);

   // Manually control the depth test function
   glDepthFunc(GL_LESS); // default comparator
   // glDepthFunc(GL_ALWAYS); 

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   Shader shader("../shaders/04.advanced/04_blending.vs",
                 "../shaders/04.advanced/04_blending.fs"
                 );

    float cube_vertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float plane_vertices[] = {
        // positions          // texture Coords
         5.0f, -0.52f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.52f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.52f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.52f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.52f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.52f, -5.0f,  2.0f, 2.0f
    };

    float grass_vertices[] = {
        // positions          // texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

   std::vector<glm::vec3> vegetation;
   vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
   vegetation.push_back(glm::vec3( 1.5f, 0.0f,  0.51f));
   vegetation.push_back(glm::vec3( 0.0f, 0.0f,  0.70f));
   vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.30f));
   vegetation.push_back(glm::vec3( 0.5f, 0.0f, -0.60f));

   // containers
   unsigned int VAO_container, VBO_container;
   glGenVertexArrays(1, &VAO_container);
   glGenBuffers(1, &VBO_container);
   glBindVertexArray(VAO_container);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_container);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   // floor
   unsigned int VAO_plane, VBO_plane;
   glGenVertexArrays(1, &VAO_plane);
   glGenBuffers(1, &VBO_plane);
   glBindVertexArray(VAO_plane);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);
   glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), &plane_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   // grass texture
   unsigned int VAO_grass, VBO_grass;
   glGenVertexArrays(1, &VAO_grass);
   glGenBuffers(1, &VBO_grass);
   glBindVertexArray(VAO_grass);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_grass);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertices), &grass_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

    shader.use();
    shader.seti("texture_sampler", 0);

    unsigned int floor_texture = texture_from_file("../texture/metal.png");
    unsigned int container_texture = texture_from_file("../texture/marble.jpg");
    unsigned int grass_texture = texture_from_file("../texture/grass.png");

   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);

      glm::mat4 projection;
      projection = glm::perspective(glm::radians(camera.zoom), (float)s_width/s_height, 0.1f, 100.0f);
      glm::mat4 view = camera.get_view_matrix();

      shader.use();

      // floor
      glBindVertexArray(VAO_plane);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, floor_texture);
      shader.setmat4("view", view);
      shader.setmat4("projection", projection);
      shader.setmat4("model", glm::mat4());
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);

      // containers
      glBindVertexArray(VAO_container);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, container_texture);
      shader.setmat4("view", view);
      shader.setmat4("projection", projection);
      glm::mat4 model;
      model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
      shader.setmat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      model = glm::mat4();
      model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
      shader.setmat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);

      // grass
      glBindVertexArray(VAO_grass);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, grass_texture);
      shader.setmat4("view", view);
      shader.setmat4("projection", projection);
      for (int i = 0; i < vegetation.size(); ++i) {
        model = glm::mat4();
        model = glm::translate(model, vegetation[i]);
        shader.setmat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
      }
      glBindVertexArray(0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   glfwTerminate();
   return 0;
}
