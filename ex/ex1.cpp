#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

const char *shader_source = "#version 330 core \n"
                            "layout (location=0) in vec3 apos; \n"
                            "void main() { \n"
                            "gl_Position = vec4(apos.x, apos.y, apos.z, 1.0); \n"
                            "} \n";

const char *fragment_source = "#version 330 core \n"
                              " out vec4 FragColor; \n"
                              " void main() { \n"
                              "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
                              " } \n";
                              
int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
   GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
   if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }   
   glfwMakeContextCurrent(window);   
   glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   float vertices[] = {
      -0.7, -0.5, 0.0,
      -0.1, -0.5, 0.0,
      -0.4,  0.0, 0.0,
       0.7, -0.5, 0.0,
       0.1, -0.5, 0.0,
       0.4,  0.0, 0.0,
   };

   unsigned int vertex_shader;
   vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex_shader, 1, &shader_source, NULL);
   glCompileShader(vertex_shader);
   int success;
   char info_log[512];
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      std::cout << "ERROR: Failed to compile vertex shader"
                << info_log 
                << std::endl;
   }

   unsigned int fragment_shader;
   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader, 1, &fragment_source, NULL);
   glCompileShader(fragment_shader);
   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      std::cout << "ERROR: Failed to compile fragment shader"
                << info_log
                << std::endl;
   }   

   unsigned int shader_program;
   shader_program = glCreateProgram();
   glAttachShader(shader_program, vertex_shader);
   glAttachShader(shader_program, fragment_shader);
   glLinkProgram(shader_program);
   glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      std::cout << "ERROR: Failed to link the shader"
                << info_log
                << std::endl;
   }
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   
   glBindVertexArray(VAO);
   
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   while (!glfwWindowShouldClose(window)) {
      glUseProgram(shader_program);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }   
   return 0;
}

