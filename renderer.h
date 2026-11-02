#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL_video.h>
#include <vector>

class Renderer {
public:
    Renderer(SDL_Window*, std::vector<float>);
    ~Renderer();
    
    void draw();

private:
    SDL_Window* _window;

    int _viewportWidth;
    int _viewportHeight;

    unsigned int _vertexBufferObject;
    unsigned int _vertexArrayObject;

    unsigned int _shaderProgram;

    const std::vector<float> _vertices;

    void compileShaders();
    void makeBuffers();
};
#endif // RENDERER_H