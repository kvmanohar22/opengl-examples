/* VERTEX SHADER VARIABLES
 * gl_Position (vec3)  : output position
 * gl_PointSize(float) : point size. You should enable first glEnable(GL_PROGRAM_POINT_SIZE)
 * gl_VertexID         : holds the current ID of the vertex we're drawing
 *                       glDrawElements: current index of vertex
 *                       glDrawElements: no. of vertices processed so far since the rendering call
 *
 * FRAGMENT SHADER VARIABLES
 * gl_FragCoord : .x, .y, .z components can be accessed
 *                if (gl_FragCoord.x < 400)
 *                  frag_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
 *                else
 *                  frag_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
 * gl_FrontFacing : 
 * gl_FrontDepth  : 
 *
 */

#include "shader.hpp"
#include "camera.hpp"

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

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>                
#endif

// for adjusting camera speed
float delta_time = 0.0f;
float last_frame = 0.0f;

// callbacks
bool first_mouse = true;
double x_last = 400.0f;
double y_last = 300.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

void mouse_callback(GLFWwindow *window, double x_new, double y_new) {
    if (first_mouse) {
        x_last = x_new;
        y_last = y_new;
        first_mouse = false;
    }
    float dx = x_new - x_last;
    float dy = y_last - y_new;
    x_last = x_new;
    y_last = y_new;

    camera.process_mouse_movement(dx, dy);
}

void process_input(GLFWwindow *window) {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    float camera_speed = 2.5f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, delta_time);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, delta_time);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, delta_time);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, delta_time);
}

void scroll_callback(GLFWwindow *window, double dx, double dy) {
    camera.process_scroll(dy);
}

int main() {
    int s_width = 1400, s_height = 720;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(s_width, s_height, "CubeMaps", NULL, NULL);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader shader1("../shaders/04.advanced/08_1.vs",
                   "../shaders/04.advanced/08_1.fs");
    Shader shader2("../shaders/04.advanced/08_2.vs",
                   "../shaders/04.advanced/08_2.fs");
    Shader shader3("../shaders/04.advanced/08_3.vs",
                   "../shaders/04.advanced/08_3.fs");
    Shader shader4("../shaders/04.advanced/08_4.vs",
                   "../shaders/04.advanced/08_4.fs");

    float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    // container
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // bind the uniform objects to a common binding point
    unsigned int binding_point = 0;
    shader1.setuniform("matrices", binding_point);
    shader2.setuniform("matrices", binding_point);
    shader3.setuniform("matrices", binding_point);
    shader4.setuniform("matrices", binding_point);

    // create the actual uniform buffer object and binding it
    // to the previously used binding point
    unsigned int ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, ubo, 0, 2 * sizeof(glm::mat4));

    // fill the buffer data
    glm::mat4 projection, view, model;

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(camera.zoom), 
                                    (float)s_width/s_height, 
                                    0.1f, 100.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        view = camera.get_view_matrix();
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindVertexArray(VAO);

        shader1.use();
        model = glm::translate(model, glm::vec3(-10.95f, 0.75f, 0.0f));
        shader1.setmat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader2.use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
        shader2.setmat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader3.use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));
        shader3.setmat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader4.use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));
        shader4.setmat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
   
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
