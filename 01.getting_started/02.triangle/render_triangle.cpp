#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>


// the callback function which is called whenever the size of the window is resized
// we need to first register this function and tell GLFW
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

int main() {
   // From the first tutorial
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
   // create window
   GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
   if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }
   
   // make the context of the window as the main context for the current thread
   glfwMakeContextCurrent(window);   
   // registering a callback function
   glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   // Define the vertices of the triangle
   float vertices[] = {
      -0.5, -0.5, 0.0,
       0.5, -0.5, 0.0,
       0.0,  0.5, 0.0
   };

   /*
       This object can store large number of vertices in the GPU's memory.
       Once the data is in the graphic's card memory, the vertex shader has instant
         access to the vertices making it extremely fast       
   */
   unsigned int VBO;
   
   /*
      This function generates one or more buffer objects
      This function returns reference IDs to access those buffers

      glGenBuffers(GLSizei size, GLuint *buffers)
      Args:
         size   : No. of buffer objects
         buffers: Array of buffer IDs
      Returns:
         An array of reference IDs which are stored in buffers
      eg:
         GLuint vbos[5];
         glGenBuffers(5, vbos);
   */
   glGenBuffers(1, &VBO);
   
   /*
      OpenGL has many types of buffers. The type of buffer associated with vertices
      is GL_ARRAY_BUFFER.
      This binds the buffer object to the current buffer type target.
      ONLY A SINGLE BUFFER CAN BE BOUND FOR EACH BUFFER TYPE
      
      glBindBuffer(GLenum target, GLuint buffer)
      Args:
         target: Bind the target buffer object (GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER)
                 are the most common ones
         buffer: Buffer objects' reference ID of the buffer you'd like to bind
      Returns:
         None
      eg:
      
   */
   glBindBuffer(GL_ARRAY_BUFFER, VBO);

   /*
      Copy the vertex data into the buffer's memory
      
      glBufferData(GLenum mode, GLsizeiptr size, const GLvoid *data, GLenum usage)
      Args:
         mode : Type of the buffer we want to copy data into
         size : size of data in bytes
         data : The actual data
         usage: specifies how the grahics card should manage the data
                GL_STATIC_DRAW  : 
                GL_DYNAIMIC_DRAW: 
                GL_STREAM_DRAW  : 
                When either of last two are chosen, graphics card places the data in memory which
                ensures fast retrieval
   */
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   /*
      ** Vertex Shader **
         Should be written in GLSL (OpenGL Shading Language
         Declare all the input vertex attributes in the vertex shader with the `in` keyword
         To set the output of the vertex shader you have to assign the position data to 
            predefined gl_Position variable which is `vec4`
   */
   const char *shader_source = "#version 330 core \n"
                               "layout (location=0) in vec3 apos; \n"
                               "void main() { \n"
                               "gl_Position = vec4(apos.x, apos.y, apos.z, 1.0); \n"      
                               "} \n";
   /*
      ** Compiling vertex shader **
   */
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

   /*
      ** Fragment shader **
      This is all about calculating the color output of the pixels
      Range is 0.0 to 1.0
      RGBA (red, green, blue, opacity)
      This shader requires only one output variable which is a vector of size 4
   */
   const char *fragment_source = "#version 330 core \n"
                                 " out vec4 FragColor; \n"
                                 " void main() { \n"
                                 "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
                                 " } \n";
                                 
   /*
      ** Compiling fragment shader ** 
   */
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

   /*
      ** Shader Program **
      Now that both the shaders are compiled, we need to link them into a shader program
         which can be used for rendering
      Shader program object is the final linked version fo multiple shaders combined
      Attach the individual shaders
      Link the shaders to create a shader program
      Check if the linking did occur properly
      Finally activate the shader program object
      Note: Every shader and rendering call after ... will use this program object 
   */
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
   glUseProgram(shader_program);
   
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);

   /*
      ** VAO **
   */
   unsigned int VAO;
   glGenVertexArrays(1, &VAO);
   glBindVertexArray(VAO);

   /*
      ** Linking Vertex Attributes **
   */
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   /*
      If you would like to unbind the VAO
         glBindVertexArray(0);
   */

   while (!glfwWindowShouldClose(window)) {
      // glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT);
   
      // glUseProgram(shader_program);
      // glBindVertexArray(VAO);   
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }   
   return 0;
}

