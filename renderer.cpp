#include "renderer.h"

#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "input_helper.h"
#include "camera.h"

using namespace std;

Renderer::Renderer(SDL_Window* window) : _window(window) {
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

void Renderer::draw(const vector<RenderableStar>& STARS, const Camera& CAMERA) {
    glViewport(0, 0, _viewportWidth, _viewportHeight);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(_shaderProgram);

    glm::mat4 view = CAMERA.getView();
    int viewLoc = glGetUniformLocation(_shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) _viewportWidth / (float) _viewportHeight, 0.1f, 100.0f);
    int projLoc = glGetUniformLocation(_shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (long) (STARS.size() * sizeof(RenderableStar)), STARS.data());

    glBindVertexArray(_vertexArrayObject);
    glPointSize(7.f);
    glDrawArrays(GL_POINTS, 0, (int) STARS.size());

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

    // TODO 6,764 is a hardcoded count from csv
    glBufferData(GL_ARRAY_BUFFER, (long) (6764 * sizeof(RenderableStar)), NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(RenderableStar), (void*)0);
    glEnableVertexAttribArray(0); 
}