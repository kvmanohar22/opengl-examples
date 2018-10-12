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
   int s_width = 1600, s_height = 2400;
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   GLFWwindow *window = glfwCreateWindow(s_width, s_height, "Crysis", NULL, NULL);
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

   Shader shader("shaders/03/shader.vs",
                 "shaders/03/shader.fs"
                 );
   Model model("models/nanosuit/nanosuit.obj");
   
   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.use();
      process_input(window);

      glm::mat4 projection;
      projection = glm::perspective(glm::radians(fov), (float)s_width/s_height, 0.1f, 100.0f);
      glm::mat4 view;
      view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
      
      shader.setmat4("projection", projection);
      shader.setmat4("view", view);
      
      glm::mat4 wmodel;
      wmodel = glm::rotate(wmodel, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
      wmodel = glm::translate(wmodel, glm::vec3(0.0f, -1.75f, 0.0f));
      wmodel = glm::scale(wmodel, glm::vec3(0.2f));
      shader.setmat4("model", wmodel);
      
      model.draw(shader);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   glfwTerminate();
   return 0;
}
