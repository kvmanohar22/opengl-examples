#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const char *vertex_shader_src1 = "#version 330 core\n"
                            "layout (location = 0) in vec3 ipos;\n"
                            "uniform mat4 view;\n"
                            "uniform mat4 model;\n"
                            "uniform mat4 projection;\n"
                            "void main() {\n"
                            "gl_Position = projection * view * model * vec4(ipos, 1.0f);\n"
                            "}\n";

const char *fragment_shader_src1 = "#version 330 core\n"
                              "out vec4 frag_color;\n"
                              "uniform vec3 obj_color;\n"
                              "uniform vec3 light_color;\n"
                              "void main() {\n"
                              "frag_color = vec4(obj_color * light_color, 1.0f);\n"
                              "}\n";

// Instead of defining again, couldn't you reuse the previous shader?
const char *vertex_shader_src2 = "#version 330 core\n"
                            "layout (location = 0) in vec3 ipos;\n"
                            "uniform mat4 view;\n"
                            "uniform mat4 model;\n"
                            "uniform mat4 projection;\n"
                            "void main() {\n"
                            "gl_Position = projection * view * model * vec4(ipos, 1.0f);\n"
                            "}\n";

const char *fragment_shader_src2 = "#version 330 core\n"
                              "out vec4 frag_color;\n"
                              "void main() {\n"
                              "frag_color = vec4(1.0f);\n"
                              "}\n";

glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

unsigned int get_shader_program(const char *vertex_shader_src,
                                const char *fragment_shader_src) {
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
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);

   return shader_program;
}

int main() {
   int s_width = 800, s_height = 600;
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   GLFWwindow *window = glfwCreateWindow(s_width, s_height, "Going 3D", NULL, NULL);
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

   // Generate shader programs
   unsigned int shader_program_obj = get_shader_program(
                     vertex_shader_src1, 
                     fragment_shader_src1);
   unsigned int shader_program_light = get_shader_program(
                     vertex_shader_src2, 
                     fragment_shader_src2);

   float vertices[] = {
       // position           // texture
       -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
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

   // For object cube
   unsigned int VBO1, VAO1;
   glGenBuffers(1, &VBO1);
   glGenVertexArrays(1, &VAO1);
   glBindVertexArray(VAO1);
   glBindBuffer(GL_ARRAY_BUFFER, VBO1);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   // For light source
   unsigned int VBO2, VAO2;
   glGenBuffers(1, &VBO2);
   glGenVertexArrays(1, &VAO2);
   glBindVertexArray(VAO2);
   glBindBuffer(GL_ARRAY_BUFFER, VBO2);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   glm::mat4 projection;
   projection = glm::perspective(glm::radians(45.0f), (float)s_width/s_height, 0.1f, 100.0f);
   glm::mat4 view;
   view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

   int model_loc, view_loc, projection_loc, obj_loc, light_loc;
   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Render light source
      glUseProgram(shader_program_light);
      glBindVertexArray(VAO1);

      glm::mat4 model;
      model = glm::translate(model, light_pos);
      model = glm::scale(model, glm::vec3(0.2f));
      model_loc = glGetUniformLocation(shader_program_light, "model");
      glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
      view_loc = glGetUniformLocation(shader_program_light, "view");
      glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
      projection_loc = glGetUniformLocation(shader_program_light, "projection");
      glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

      glDrawArrays(GL_TRIANGLES, 0, 36);         
      glBindVertexArray(0);

      // Render object
      glUseProgram(shader_program_obj);
      glBindVertexArray(VAO2);

      glm::mat4 model_obj;
      model_obj = glm::rotate(model_obj, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      model_obj = glm::rotate(model_obj, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      model_loc = glGetUniformLocation(shader_program_obj, "model");
      glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_obj));
      view_loc = glGetUniformLocation(shader_program_obj, "view");
      glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
      projection_loc = glGetUniformLocation(shader_program_obj, "projection");
      glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
      obj_loc = glGetUniformLocation(shader_program_obj, "obj_color");
      glUniform3f(obj_loc, 1.0f, 0.5f, 0.31f);
      light_loc = glGetUniformLocation(shader_program_obj, "light_color");
      glUniform3f(light_loc, 1.0f, 1.0f, 1.0f); 

      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);

      // IO / Swap buffers
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

