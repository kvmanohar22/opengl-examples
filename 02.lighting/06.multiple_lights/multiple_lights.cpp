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

// for adjusting camera speed
float delta_time = 0.0f;
float last_frame = 0.0f;

// for camera related positions
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// callbacks
bool first_mouse = true;
double yaw = -90.0f;
double pitch = 0.0f;
double x_last = 400.0f;
double y_last = 300.0f;
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
   
   float sensitivity = 0.05;
   dx *= sensitivity;
   dy *= sensitivity;
   
   yaw += dx;
   pitch += dy;
   
   if (pitch > 89.0f)
      pitch = 89.0f;
   if (pitch < -89.0f)
      pitch = -89.0f;
   
   glm::vec3 front;
   front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   front.y = sin(glm::radians(pitch));
   front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   camera_front = glm::normalize(front);   
}

void process_input(GLFWwindow *window) {
   float current_frame = glfwGetTime();
   delta_time = current_frame - last_frame;
   last_frame = current_frame;
   float camera_speed = 2.5f * delta_time;

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera_pos += camera_speed * camera_front;
   else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera_pos -= camera_speed * camera_front;
   else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
   else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
}

float fov = 45.0f;
void scroll_callback(GLFWwindow *window, double dx, double dy) {
   if (fov >= 1.0f && fov <= 45.0f)
      fov -= dy;
   if (fov < 1.0f)
      fov = 1.0f;
   if (fov > 45.0f)
      fov = 45.0f;
}

int main() {
   int s_width = 800, s_height = 600;
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   GLFWwindow *window = glfwCreateWindow(s_width, s_height, "Lighting", NULL, NULL);
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
   glfwMakeContextCurrent(window);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   // Generate shader programs
   unsigned int shader_program_obj, shader_program_light;
   Shader obj_shader("shaders/02/multiple_lights/obj_shader.vs",
                     "shaders/02/multiple_lights/obj_shader.fs"
                    );
   Shader light_shader("shaders/02/multiple_lights/light_shader.vs",
                       "shaders/02/multiple_lights/light_shader.fs"
                      );

   float vertices[] = {
       // positions          // normals           // texture coords
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
   };

   glm::vec3 cube_positions[] = {
     glm::vec3( 0.0f,  0.0f,  0.0f), 
     glm::vec3( 2.0f,  5.0f, -15.0f), 
     glm::vec3(-1.5f, -2.2f, -2.5f),  
     glm::vec3(-3.8f, -2.0f, -12.3f),  
     glm::vec3( 2.4f, -0.4f, -3.5f),  
     glm::vec3(-1.7f,  3.0f, -7.5f),  
     glm::vec3( 1.3f, -2.0f, -2.5f),  
     glm::vec3( 1.5f,  2.0f, -2.5f), 
     glm::vec3( 1.5f,  0.2f, -1.5f), 
     glm::vec3(-1.3f,  1.0f, -1.5f)  
   };

   glm::vec3 point_light_positions[] = {
	   glm::vec3( 0.7f,  0.2f,  2.0f),
	   glm::vec3( 2.3f, -2.7f, -4.0f),
	   glm::vec3(-4.0f,  2.0f, -12.0f),
	   glm::vec3( 0.0f,  0.0f, -3.0f)
   };  

   // For object cube
   unsigned int VBO1, VAO1;
   glGenBuffers(1, &VBO1);
   glGenVertexArrays(1, &VAO1);
   glBindVertexArray(VAO1);
   glBindBuffer(GL_ARRAY_BUFFER, VBO1);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   // For light source
   unsigned int VBO2, VAO2;
   glGenBuffers(1, &VBO2);
   glGenVertexArrays(1, &VAO2);
   glBindVertexArray(VAO2);
   glBindBuffer(GL_ARRAY_BUFFER, VBO2);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   // Texture for diffuse lighting
   unsigned int TEX1;
   glGenTextures(1, &TEX1);
   glBindTexture(GL_TEXTURE_2D, TEX1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   int width, height, nchannels;
   unsigned char *data = stbi_load("imgs/container2.png", &width, &height, &nchannels, 0);
   if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);      
   } else {
      std::cout << "Couldn't load the texture image!"
                << std::endl;
      return -1;
   }
   stbi_image_free(data);

   // Texture for specular lighting
   unsigned int TEX2;
   glGenTextures(1, &TEX2);
   glBindTexture(GL_TEXTURE_2D, TEX2);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   data = stbi_load("imgs/container2_specular.png", &width, &height, &nchannels, 0);
   if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);      
   } else {
      std::cout << "Couldn't load the texture image!"
                << std::endl;
      return -1;
   }
   stbi_image_free(data);

   obj_shader.use();
   obj_shader.seti("material.diffuse", 0);
   obj_shader.seti("material.specular", 1);
   obj_shader.setf("material.shininess", 64.0f);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, TEX1);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, TEX2);

   glm::vec3 light_ambience(0.05f);
   glm::vec3 light_diffuse(0.8f);
   glm::vec3 light_specular(0.5f);

   float c = 1.0;
   float l = 0.7;
   float q = 1.8;

   glm::mat4 projection;
   projection = glm::perspective(glm::radians(45.0f), (float)s_width/s_height, 0.1f, 100.0f);

   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      process_input(window);
      glm::mat4 view;
      view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

      // Render all light sources
      light_shader.use();
      glBindVertexArray(VAO1);

      light_shader.setmat4("view", view);
      light_shader.setmat4("projection", projection);
      for (int i = 0; i < 4; ++i) {
         glm::mat4 model;
         if (i == 4) {
            float radius = 3.0f;
            float posX = sin(glfwGetTime()) * radius;
            float posZ = cos(glfwGetTime()) * radius;
            model = glm::translate(model, glm::vec3(posX, 0, posZ));
            point_light_positions[2] = glm::vec3(posX, 0, posZ);
            model = glm::scale(model, glm::vec3(0.2f));
         } else {
            model = glm::translate(model, point_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
         }
         light_shader.setmat4("model", model);
         glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      glBindVertexArray(0);

      // Render objects
      obj_shader.use();
      glBindVertexArray(VAO2);

      obj_shader.setvec3("_point_sources[0].position", point_light_positions[0]);
      obj_shader.setvec3("_point_sources[1].position", point_light_positions[1]);
      obj_shader.setvec3("_point_sources[2].position", point_light_positions[2]);
      obj_shader.setvec3("_point_sources[3].position", point_light_positions[3]);
   
      obj_shader.setf("_point_sources[0].constant", 1.0f);
      obj_shader.setf("_point_sources[1].constant", 1.0f);
      obj_shader.setf("_point_sources[2].constant", 1.0f);
      obj_shader.setf("_point_sources[3].constant", 1.0f);

      obj_shader.setf("_point_sources[0].linear", l);
      obj_shader.setf("_point_sources[1].linear", l);
      obj_shader.setf("_point_sources[2].linear", l);
      obj_shader.setf("_point_sources[3].linear", l);

      obj_shader.setf("_point_sources[0].quadratic", q);
      obj_shader.setf("_point_sources[1].quadratic", q);
      obj_shader.setf("_point_sources[2].quadratic", q);
      obj_shader.setf("_point_sources[3].quadratic", q);

      obj_shader.setvec3("_point_sources[0].ambient", light_ambience);
      obj_shader.setvec3("_point_sources[1].ambient", light_ambience);
      obj_shader.setvec3("_point_sources[2].ambient", light_ambience);
      obj_shader.setvec3("_point_sources[3].ambient", light_ambience);

      obj_shader.setvec3("_point_sources[0].diffuse", light_diffuse);
      obj_shader.setvec3("_point_sources[1].diffuse", light_diffuse);
      obj_shader.setvec3("_point_sources[2].diffuse", light_diffuse);
      obj_shader.setvec3("_point_sources[3].diffuse", light_diffuse);

      obj_shader.setvec3("_point_sources[0].specular", light_specular);
      obj_shader.setvec3("_point_sources[1].specular", light_specular);
      obj_shader.setvec3("_point_sources[2].specular", light_specular);
      obj_shader.setvec3("_point_sources[3].specular", light_specular);
   
      obj_shader.setvec3("_dir_source.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
      obj_shader.setvec3("_dir_source.ambient", glm::vec3(0.05f));
      obj_shader.setvec3("_dir_source.diffuse", glm::vec3(0.4f));
      obj_shader.setvec3("_dir_source.specular", glm::vec3(0.5f));
   
      obj_shader.setvec3("view_pos", camera_pos);
      obj_shader.setmat4("view", view);
      obj_shader.setmat4("projection", projection);
      for (int i = 0; i < 10; ++i) {
         glm::mat4 model;
         model = glm::translate(model, cube_positions[i]);
         model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
         obj_shader.setmat4("model", model);
         
         glDrawArrays(GL_TRIANGLES, 0, 36);            
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glDeleteVertexArrays(1, &VAO1);
   glDeleteBuffers(1, &VBO1);
   glDeleteVertexArrays(1, &VAO2);
   glDeleteBuffers(1, &VBO2);
   glfwTerminate();

   return 0;
}
