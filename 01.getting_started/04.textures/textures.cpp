#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/*
 * Texture: - Is a 2D image (1D, 3D also exist) used to add detail to an object
 *          - These usually range from (0, 0) to (1, 1)
 *          - The axes are (s, t, r)

 * ** Setting the texture parameters **
 * GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_REPEAT, GL_CLAMP_TO_BORDER
 * glTexParameteri(GL_TEXTURE_2D,    GL_TEXTURE_WRAP_S,        GL_MIRRORED_REPEAT)
 * glTexParameteri(GL_TEXTURE_2D,    GL_TEXTURE_WRAP_T,        GL_MIRRORED_REPEAT)
 *                //texture target   // which option and axis  // value
 * float border_color[] = {1f, 1f, 1f, 1f};
 * glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
 
 * ** Texture Filtering **
 * GL_LINEAR (uses bilinear interpolation), GL_NEAREST (nearest neighbor, default)
 * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

 * ** Mipmaps **
 * There are problems while mapping a high-reslution texture to small objects.
 * Consider objects which are far away and produce only few fragments. OpenGL
 *    has problems in retreiving colors from texture for fragments which have 
 *    very small size.
 * `glGenerateMipmaps` generates the mipmap after we have created the texture
 * You can also interpolate b/w mipmaps
 * glTexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINERA_MIPMAP_LINEAR);
 * There is no point in using mipmaps for `MAG_FILTER` because texture gets magnified

 * ** Loading a texture **
 * #define STB_IMAGE_IMPLEMENTATION
 * #include "stb_image.h"
 * int width, height, nchannels;
 * unsigned char *data = stbi_load(
 *             "/path/to/image.xyz", &width,
 *             &height, &nchannels, 0);
 
 * ** Generating a texture **
 * 
 */

void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

const char *shader_source = "#version 330 core \n"
                            "layout (location=0) in vec3 pos; \n"
                            "layout (location=1) in vec3 col;\n"
                            "layout (location=2) in vec2 tex; \n"
                            "out vec3 f_col;\n"
                            "out vec2 f_tex;\n"
                            "void main() { \n"
                            "gl_Position = vec4(pos, 1.0); \n"
                            "f_col = col;\n"
                            "f_tex = tex;\n"
                            "} \n";

const char *fragment_source = "#version 330 core \n"
                              "out vec4 FragColor; \n"
                              "in vec3 f_col;\n"
                              "in vec2 f_tex;\n"
                              "uniform sampler2D our_tex"
                              " void main() { \n"
                              "   FragColor = texture(our_tex, f_tex); \n"
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

   float vertices[] = {
      // position       // color           // texture coords
      -0.7, -0.7, 0.0,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
       0.7, -0.7, 0.0,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
       0.0,  0.7, 0.0,  0.0f, 0.0f, 1.0f,  0.5f, 0.5f
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
      std::cout << "Couldn't load the texture image!"
                << std::endl;
      return -1;
   }
   stbi_image_free(data);
   
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   while (!glfwWindowShouldClose(window)) {
      glUseProgram(shader_program);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glDeleteBuffers(1, &VBO);
   glDeleteVertexArrays(1, &VAO);
   glfwTerminate();
   return 0;
}
