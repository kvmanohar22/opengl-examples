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

#include <shader.hpp>
#include <mesh.hpp>
#include <camera.hpp>
#include <utils.hpp>
#include <model.hpp>

// for adjusting camera speed
float delta_time = 0.0f;
float last_frame = 0.0f;

// callbacks
bool first_mouse = true;
double x_old = 400.0f;
double y_old = 300.0f;

Camera camera(glm::vec3(0.0f, 10.0f, 50.0f));

/**************************** MOUSE CALLBACK ****************************/
void mouse_callback(GLFWwindow *window, 
                    double x_new, double y_new) {
    if (first_mouse) {
        x_old = x_new;
        y_old = y_new;
        first_mouse = false;
    }
    float dx = x_new - x_old;
    float dy = y_old - y_new;
    x_old = x_new;
    y_old = y_new;

    camera.process_mouse_movement(dx, dy);
}

/**************************** SCROLL CALLBACK ****************************/
void scroll_callback(GLFWwindow *window, double dx, double dy) {
    camera.process_scroll(dy);
}

int main() {
   int s_width = 1800, s_height = 1000;
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

   glEnable(GL_DEPTH_TEST);
   glfwMakeContextCurrent(window);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   Shader shader("../shaders/04.advanced/10_explode.vs",
                 "../shaders/04.advanced/10_explode.fs"
                 );

   Model model_planet("../models/planet/planet.obj");
   Model model_rock("../models/rock/rock.obj");

   unsigned int amount = 2000;
   glm::mat4 *model_matrices = new glm::mat4[amount];
   srand(glfwGetTime());
   float radius = 30.0f;
   float offset = 2.5f;

   for (unsigned int i = 0; i < amount; ++i) {
      glm::mat4 model;

      // 1. Translate
      float displacement;
      float angle = (float)i / (float)amount * 360.0f;
      displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
      float x = sin(angle) * radius + displacement;
      displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
      float y = displacement * 0.4f;
      displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
      float z = cos(angle) * radius + displacement;
      model = glm::translate(model, glm::vec3(x, y, z));

      // 2. Scale
      float scale = (rand() % 20) / 100.0f + 0.05f;
      model = glm::scale(model, glm::vec3(scale));

      // 3. Rotation
      float r_angle = (rand() % 360);
      model = glm::rotate(model, r_angle, glm::vec3(0.4f, 0.6f, 0.8f));

      model_matrices[i] = model;
   }

   while (!glfwWindowShouldClose(window)) {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      utils::process_input(window, last_frame, delta_time, camera);

      glm::mat4 projection;
      projection = glm::perspective(glm::radians(camera.zoom), 
                                    (float)s_width/s_height, 
                                    0.1f, 100.0f);
      float _R = 65.0f;
      float _x = sin(glfwGetTime()) * _R;
      float _z = cos(glfwGetTime()) * _R;
      camera.update_position(glm::vec3(_x, 30.0f, _z));
      glm::mat4 view = camera.get_view_matrix(MOVING);

      // draw planet
      shader.use();
      shader.setmat4("projection", projection);
      shader.setmat4("view", view);
      glm::mat4 wmodel;
      wmodel = glm::translate(wmodel, glm::vec3(0.0f, -3.0f, 0.0f));
      wmodel = glm::scale(wmodel, glm::vec3(0.4f));
      shader.setmat4("model", wmodel);
      model_planet.draw(shader);

      // draw asteroids
      for (size_t i = 0; i < amount; ++i) {
         shader.setmat4("model", model_matrices[i]);
         model_rock.draw(shader);
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   glfwTerminate();
   return 0;
}
