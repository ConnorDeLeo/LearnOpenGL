#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

// globals
float r = 0.0f;
float g = 0.0f;
float b = 0.0f;

std::minstd_rand rng(std::random_device{}());
std::uniform_real_distribution<float> floatDist(-0.01f, 0.01f);

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// processing functions
void processInput(GLFWwindow *window) {

    // check for escape
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // close window
        glfwSetWindowShouldClose(window, true);
    }
}

// rendering functions
std::vector<float> cycleColors(float r, float g, float b) {
    std::vector<float> rgb;

    r += floatDist(rng);
    g += floatDist(rng);
    b += floatDist(rng);

    if(r >= 1.0f) {
        r = 1.0f;
    }
    else if(r <= 0.0f) {
        r = 0.0f;
    }

    if(g >= 1.0f) {
        g = 1.0f;
    }
    else if(g <= 0.0f) {
        g = 0.0f;
    }

    if(b >= 1.0f) {
        b = 1.0f;
    }
    else if(b <= 0.0f) {
        b = 0.0f;
    }

    rgb.push_back(r);
    rgb.push_back(g);
    rgb.push_back(b);

    return rgb;
}

// main
int main() {
    // init glfw
    if(!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
    }

    // ver 3.4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // use core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // create window obj
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    // kill glfw if window isn't created
    if(window == nullptr) {
        std::cout << "Failed creation of GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // make window the current context
    glfwMakeContextCurrent(window);

    // initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render a viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while(!glfwWindowShouldClose(window)) {
        // process any user inputs
        processInput(window);

        // rendering
        std::vector<float> rgb = cycleColors(r, g, b);
        r = rgb[0];
        g = rgb[1];
        b = rgb[2];

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // update graphics
        glfwSwapBuffers(window);

        // check events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
