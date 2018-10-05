#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

const char *shader_source = "#version 330 core \n"
                            "layout (location=0) in vec3 apos; \n"
                            "void main() { \n"
                            "gl_Position = vec4(apos.x, apos.y, apos.z, 1.0); \n"
                            "} \n";

const char *fragment_source1 = "#version 330 core \n"
                              " out vec4 FragColor; \n"
                              " void main() { \n"
                              "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
                              " } \n";

const char *fragment_source2 = "#version 330 core \n"
                              " out vec4 FragColor; \n"
                              " void main() { \n"
                              "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f); \n"
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

   float vertices1[] = {
      -0.7, -0.5, 0.0,
      -0.1, -0.5, 0.0,
      -0.4,  0.0, 0.0
   };
   float vertices2[] = {
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

   unsigned int fragment_shader1 = glCreateShader(GL_FRAGMENT_SHADER);;
   glShaderSource(fragment_shader1, 1, &fragment_source1, NULL);
   glCompileShader(fragment_shader1);
   glGetShaderiv(fragment_shader1, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(fragment_shader1, 512, NULL, info_log);
      std::cout << "ERROR: Failed to compile fragment shader"
                << info_log
                << std::endl;
   }   

   unsigned int fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);;
   glShaderSource(fragment_shader2, 1, &fragment_source2, NULL);
   glCompileShader(fragment_shader2);
   glGetShaderiv(fragment_shader2, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(fragment_shader2, 512, NULL, info_log);
      std::cout << "ERROR: Failed to compile fragment shader"
                << info_log
                << std::endl;
   }   

   unsigned int shader_program1 = glCreateProgram();;
   glAttachShader(shader_program1, vertex_shader);
   glAttachShader(shader_program1, fragment_shader1);
   glLinkProgram(shader_program1);
   glGetProgramiv(shader_program1, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(shader_program1, 512, NULL, info_log);
      std::cout << "ERROR: Failed to link the shader"
                << info_log
                << std::endl;
   }
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader1);
  
   unsigned int shader_program2 = glCreateProgram();;
   glAttachShader(shader_program2, vertex_shader);
   glAttachShader(shader_program2, fragment_shader2);
   glLinkProgram(shader_program2);
   glGetProgramiv(shader_program2, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(shader_program2, 512, NULL, info_log);
      std::cout << "ERROR: Failed to link the shader"
                << info_log
                << std::endl;
   }
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader2);

   // VAO1
   unsigned int VBO1, VAO1;
   glGenVertexArrays(1, &VAO1);
   glGenBuffers(1, &VBO1);
   glBindVertexArray(VAO1);
   glBindBuffer(GL_ARRAY_BUFFER, VBO1);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // VAO2
   unsigned int VBO2, VAO2;
   glGenVertexArrays(1, &VAO2);
   glGenBuffers(1, &VBO2);
   glBindVertexArray(VAO2);
   glBindBuffer(GL_ARRAY_BUFFER, VBO2);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   while (!glfwWindowShouldClose(window)) {
      process_input(window);
   
      // Draw the 1st triangle
      glUseProgram(shader_program1);
      glBindVertexArray(VAO1);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      // Draw the 2nd triangle
      glUseProgram(shader_program2);
      glBindVertexArray(VAO2);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }   

   glDeleteVertexArrays(1, &VAO1);
   glDeleteVertexArrays(1, &VAO2);
   
   glDeleteBuffers(1, &VBO1);
   glDeleteBuffers(1, &VBO2);

   glfwTerminate();
   return 0;
}

