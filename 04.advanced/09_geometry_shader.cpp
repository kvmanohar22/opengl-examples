/*
 *
 *
 *
 */

#include "shader.hpp"
#include "camera.hpp"
#include "utils.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
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
double x_old = 400.0f;
double y_old = 300.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
    glEnable(GL_PROGRAM_POINT_SIZE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Naive geometry shader
    // Shader shader("../shaders/04.advanced/09.vs", 
    //               "../shaders/04.advanced/09.fs",
    //               "../shaders/04.advanced/09.gs");

    // 4 houses shader
    Shader shader("../shaders/04.advanced/09.vs", 
                  "../shaders/04.advanced/09.fs",
                  "../shaders/04.advanced/09_house.gs");

    // naive
    // float points[] = {
    //     -0.5f,  0.5f,
    //      0.5f,  0.5f,
    //      0.5f, -0.5f,
    //     -0.5f, -0.5f
    // };

    float points[] = {
        // position   // color
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f
    };

    // container
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        utils::process_input(window, last_frame, delta_time, camera);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        shader.use();
        glDrawArrays(GL_POINTS, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
