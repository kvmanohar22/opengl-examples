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
                            "out vec2 tex;\n"                              
                            "uniform mat4 view;\n"
                            "uniform mat4 model;\n"
                            "uniform mat4 projection;\n"
                            "void main() {\n"
                            "gl_Position = projection * view * model * vec4(ipos, 1.0f);\n"
                            "tex = itex;\n"
                            "}\n";

const char *fragment_shader_src = "#version 330 core\n"
                              "out vec4 frag_color;\n"
                              "in vec2 tex;\n"
                              "uniform sampler2D stex1;"
                              "uniform sampler2D stex2;"
                              "void main() {\n"
                              "frag_color = mix(texture(stex1, tex), texture(stex2, tex), 0.4);\n"
                              "}\n";

// for adjusting camera speed
float delta_time = 0.0f;
float last_frame = 0.0f;

// for camera related positions
glm::vec3 camera_pos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f, 0.0f);

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
   float dy = y_new - y_last;
   // float dy = y_last - y_new;
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
   float camera_speed = 2.5f / delta_time;

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
   GLFWwindow *window = glfwCreateWindow(s_width, s_height, "Going 3D", NULL, NULL);
   if (window == NULL) {
      std::cout << "Couldn't create window!";
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }
   
   int success;
   char info_log[512];

   // Enable the depth buffer
   glEnable(GL_DEPTH_TEST);

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
       // position           // texture
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

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   unsigned int TEX1;
   glGenTextures(1, &TEX1);
   glBindTexture(GL_TEXTURE_2D, TEX1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   int width, height, nchannels;
   unsigned char *data;
   data = stbi_load("../imgs/container.jpg", &width, &height, &nchannels, 0);
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

   unsigned int TEX2;
   glGenTextures(1, &TEX2);
   glBindTexture(GL_TEXTURE_2D, TEX2);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   stbi_set_flip_vertically_on_load(true); 
   data = stbi_load("../imgs/awesomeface.png", &width, &height, &nchannels, 0);
   if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   } else {
      std::cout << "Couldn't load the texture!"
                << std::endl;
      glfwTerminate();
      return -1;
   }
   stbi_image_free(data);

   glBindVertexArray(VAO);
   
   // VBO Buffer data
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   // Position coords
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   // Texture coords
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   glUseProgram(shader_program);
   glUniform1i(glGetUniformLocation(shader_program, "stex1"), 0);
   glUniform1i(glGetUniformLocation(shader_program, "stex2"), 1);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, TEX1);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, TEX2);

   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(shader_program);
      glBindVertexArray(VAO);
      for (int i = 0; i < 10; ++i) {
         process_input(window);
         glm::mat4 model;
         model = glm::translate(model, cube_positions[i]);

         glm::mat4 projection;
         projection = glm::perspective(glm::radians(fov), (float)s_width/s_height, 0.1f, 100.0f);

         glm::mat4 view;
         view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

         int model_loc = glGetUniformLocation(shader_program, "model");
         glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
         int view_loc = glGetUniformLocation(shader_program, "view");
         glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
         int projection_loc = glGetUniformLocation(shader_program, "projection");
         glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

         glDrawArrays(GL_TRIANGLES, 0, 36);         
      }
      glBindVertexArray(0);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glfwTerminate();

   return 0;     
}

