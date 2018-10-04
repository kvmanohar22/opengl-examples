#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>


// the callback function which is called whenever the size of the window is resized
// we need to first register this function and tell GLFW
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

int main() {
 
   // Initialize glfw window manager
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
      
   // tell openGL the size of the rendering window
   // first two args are lower left corner's location and 
   // the other two are width and height
   // - Processed coordinates in openGL are (-1, 1)
   // glViewport(0, 0, 800, 600);
   
   // registering a callback function
   glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
   
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }
   
   // keep rendering the window until we wish to close
   while (!glfwWindowShouldClose(window)) {
      // at each rendering iteration if you want to check for some events
      // do that by calling some GLFW's input functions     
      process_input(window);
      
      // At the start of each render iteration you would want to clear the screen so
      // this is put in the render loop
      // clear the buffer
      // glClearColor -> state-setting function
      // glClear      -> state-using function
      // glClear function takes in buffer bits to specify which buffer we would like to clear
      // GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
      glClearColor(.2f, .3f, .3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   // Terminate
   glfwTerminate();
   return 0;
}

