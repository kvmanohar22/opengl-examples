#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


const char *vertex_shader_src = "#version 330 core\n"
                            "layout (location = 0) in vec3 pos;\n"
                            "void main() {\n"
                            "gl_Position = vec4(pos, 1.0);\n"
                            "}\n";
                            
/*
 * Uniforms are usually used for setting attributes that might change in render iterations
 * These are also used for interchanging data between the our application and shaders
 * Now, the above declaration sets the same color for each vertex attribute of the figure
 * What if you wanted to set a different colro for each vertex?
 * In which case we would have to declare as many uniforms as the number of vertices
 * A better way would be to include more data in the vertex attribute itself!
 */

const char *fragment_shader_src = "#version 330 core\n"
                              "out vec4 frag_color;\n"
                              "uniform vec4 our_color;\n"
                              "void main() {\n"
                              "frag_color = our_color;\n"
                              "}\n";


int main() {
   /*
      - Shaders can communicate with one another provided they have similar
        data types i.e, they must have same types and same names as well
      - eg: of a vertex shader
        #version 330 core
        layout (position = 0) in vec3 pos;
        out vec4 vertex_color;
        void main() {
           gl_Position  = vec4(pos, 1.0);
           vertex_color = vec4(0.5, 0.0, 0.0, 1.0);
        }
      - eg: of a fragment shader
        #version 330 core   
        out vec4 frag_color;
        in vec4 vertex_color;
        void main() {
           frag_color = vertex_color;
        }
   */
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
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);
   
   float vertices[] = {
      -0.7, -0.7, 0.0,
       0.7, -0.7, 0.0,
       0.0,  0.7, 0.0,      
   };
   
   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   while (!glfwWindowShouldClose(window)) {
      glUseProgram(shader_program);

      // Set the fragment shader color at run-time
      float time_val = glfwGetTime();
      float green_val = sin(time_val)/2.0f + 0.5f;
      int vertex_color_loc = glGetUniformLocation(shader_program, "our_color");
      glUniform4f(vertex_color_loc, 0.0f, green_val, 0.0f, 1.0f);

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
