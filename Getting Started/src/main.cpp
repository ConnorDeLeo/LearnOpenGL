#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>

#include <vector>
#include <string>

// globals
// color vars
float r = 0.5f;
float g = 0.5f;
float b = 0.5f;

// vertexes
float vertices[] {
    -0.5f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f,
    0.5f, 0.0f, 0.0f
};

// shaders
const char* shaderPaths[] {
    "shaders/vertexShaderSource.GLSL",
    "shaders/fragmentShaderSource.GLSL"
};

// functions
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

// read a file and output its constants as a c-style string
std::string readFile(const char* path) {
    static std::string source;
    std::ifstream file(path);

    if(!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return nullptr;
    }

    // read via stringbuffer
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
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

    // load shader files
    std::string vertexStringSource = readFile(shaderPaths[0]);
    std::string fragmentStringSource = readFile(shaderPaths[1]);
    const char* vertexShaderSource = vertexStringSource.c_str();
    const char* fragmentShaderSource = fragmentStringSource.c_str();

    // create shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // source shaders
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

    // compile shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // check compilation success
    int vertexSuccess;
    char vertexInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);

    if(!vertexSuccess) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, vertexInfoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
    }

    int fragmentSuccess;
    char fragmentInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);

    if(!fragmentSuccess) {
        glGetShaderInfoLog(fragmentSuccess, 512, nullptr, fragmentInfoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
    }

    // create a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // attach and link shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check linking
    int linkSuccess;
    char linkInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);

    if(!linkSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, linkInfoLog);
        std::cerr << "ERROR::SHADERPROGRAM::LINKING_FAILED\n" << linkInfoLog << std::endl;
    }

    // kill shaders after linking
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    // create and bind a vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // generate a buffer for vertex data
    unsigned int VBO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // enable vertex attribute arrays
    glEnableVertexAttribArray(0);

    // use shader program
    glUseProgram(shaderProgram);

    // render a viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while(!glfwWindowShouldClose(window)) {
        // process any user inputs
        processInput(window);

        // rendering
        // colors
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // update graphics
        glfwSwapBuffers(window);

        // check events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
