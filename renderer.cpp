#include "renderer.h"

#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "input_helper.h"

using namespace std;

Renderer::Renderer(SDL_Window* window, vector<float> vertices) : _window(window), _vertices(vertices) {
    SDL_GetWindowSizeInPixels(window, &_viewportWidth, &_viewportHeight);

    compileShaders();
    makeBuffers();
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &_vertexBufferObject);
    glDeleteVertexArrays(1, &_vertexArrayObject);
    glUseProgram(0); // Stop using the program before deleting it
    glDeleteProgram(_shaderProgram);
}

void Renderer::draw() {
    glViewport(0, 0, _viewportWidth, _viewportHeight);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(_shaderProgram);
    glBindVertexArray(_vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(_window);
}

void Renderer::resizeViewport() {
    SDL_GetWindowSizeInPixels(_window, &_viewportWidth, &_viewportHeight);
}

void Renderer::compileShaders() {
    InputHelper inputHelper;

    const string vertexShaderSource = inputHelper.readFile("shaders/star.vert");
    const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    const string fragmentShaderSource = inputHelper.readFile("shaders/star.frag");
    const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    _shaderProgram = glCreateProgram();

    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(_shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::makeBuffers() {
    glGenBuffers(1, &_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);

    glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);

    glBufferData(GL_ARRAY_BUFFER,  (long) (_vertices.size() * sizeof(float)), _vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
}