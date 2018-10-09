#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const char *vertex_shader_src = "#version 330 core\n"
                            "layout (location = 0) in vec3 ipos;\n"
                            "layout (location = 1) in vec2 itex;\n"
                            "out vec2 tex; \n"
                            "uniform mat4 transform;\n"
                            "void main() {\n"
                            "gl_Position = transform * vec4(ipos, 1.0f);\n"
                            // "gl_Position = vec4(ipos, 1.0f);\n"
                            "tex = itex;\n"
                            "}\n";
                            
const char *fragment_shader_src = "#version 330 core\n"
                              "out vec4 frag_color;\n"
                              "in vec2 tex;\n"
                              "uniform sampler2D stex;"
                              "void main() {\n"
                              "frag_color = texture(stex, tex);\n"
                              "}\n";

int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   GLFWwindow *window = glfwCreateWindow(800, 600, "Uniform Shaders", NULL, NULL);
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
   
   int success;
   char info_log[512];

   // Vertex shader
   unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
   glCompileShader(vertex_shader);
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      std::cout << "Couldn't compile vertex shader"
                << info_log
                << std::endl;
   }
   // Fragment shader
   unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
   glCompileShader(fragment_shader);
   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      std::cout << "Couldn't compile fragment shader"
                << info_log
                << std::endl;
   }
   // Shader Program
   unsigned int shader_program = glCreateProgram();
   glAttachShader(shader_program, vertex_shader);
   glAttachShader(shader_program, fragment_shader);
   glLinkProgram(shader_program);
   glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      std::cout << "Couldn't link shader program"
                << info_log
                << std::endl;
   }
   glUseProgram(shader_program);
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);
   
   float vertices[] = {
      // position        // texture coords
      -0.7, -0.7, 0.0,   0.0, 0.0,
       0.7, -0.7, 0.0,   1.0, 1.0,
       0.0,  0.7, 0.0,   0.5, 0.5
   };
   
   unsigned int VBO, VAO, TEX;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glGenTextures(1, &TEX);
   glBindTexture(GL_TEXTURE_2D, TEX);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   int width, height, nchannels;
   unsigned char *data = stbi_load("imgs/container.jpg", &width, &height, &nchannels, 0);
   if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   } else {
      std::cout << "Couldn't load the texture!"
                << std::endl;
      glfwTerminate();
      return -1;
   }
   stbi_image_free(data);

   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   glm::mat4 trans;
   // Uncomment this to make the transformation static
   // trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
   // trans = glm::scale(trans, glm::vec3(0.2, 0.2, 0.2));
   // unsigned int transform_loc = glGetUniformLocation(shader_program, "transform");
   // glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT);

      trans = glm::translate(trans, glm::vec3(0.0, 0.0, 0.0));
      trans = glm::rotate(trans, glm::radians((float)glfwGetTime()/(float)100.0), glm::vec3(0.0f, 0.0f, 1.0f));

      glUseProgram(shader_program);
      unsigned int transform_loc = glGetUniformLocation(shader_program, "transform");
      glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glfwTerminate();
   
   return 0;
}
