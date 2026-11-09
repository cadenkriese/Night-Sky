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

const float VERTICAL_FOV = 45.f; // In degrees

Renderer::Renderer(SDL_Window* window) : _window(window) {
    SDL_GetWindowSizeInPixels(window, &_viewportWidth, &_viewportHeight);

    makeShaderProgram();
    makeBuffers();
    glEnable(GL_PROGRAM_POINT_SIZE); // Allow modifying point size from vertex shader
    glEnable(GL_BLEND);              // Enable blending to let overlapping stars accumulate light
    glBlendFunc(GL_ONE, GL_ONE);     // Even (1:1) blending
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

    glm::mat4 projection = glm::perspective(glm::radians(VERTICAL_FOV), (float) _viewportWidth / (float) _viewportHeight, 0.1f, 100.0f);
    int projLoc = glGetUniformLocation(_shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (long) (STARS.size() * sizeof(RenderableStar)), STARS.data());

    glBindVertexArray(_vertexArrayObject);
    glDrawArrays(GL_POINTS, 0, (int) STARS.size());

    SDL_GL_SwapWindow(_window);
}

void Renderer::resizeViewport() {
    SDL_GetWindowSizeInPixels(_window, &_viewportWidth, &_viewportHeight);
}

unsigned int Renderer::compileShader(string file, GLenum type) {
    InputHelper inputHelper;
    const string shaderSource = inputHelper.readFile(file);
    const char* shaderSourceCStr = shaderSource.c_str();
    unsigned int shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSourceCStr, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    return shader;
}

void Renderer::makeShaderProgram() {
    unsigned int vertexShader = compileShader("shaders/star.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader("shaders/star.frag", GL_FRAGMENT_SHADER);

    _shaderProgram = glCreateProgram();

    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);

    int success;
    char infoLog[512];
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

    // Vertex buffer structure
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(RenderableStar), (void*) 0);               // Altitude & Azimuth
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(RenderableStar), (void*) (2 * sizeof(float))); // Magnitude
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(RenderableStar), (void*) (3 * sizeof(float))); // Color

    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1); 
    glEnableVertexAttribArray(2);
}