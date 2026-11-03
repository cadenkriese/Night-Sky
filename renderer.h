#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <SDL3/SDL_video.h>

class Renderer {
public:
    Renderer(SDL_Window*, std::vector<float>);
    ~Renderer();
    
    /**
     * @brief renders a frame using OpenGL and swaps it to the stored SDL_Window*
     */
    void draw();

    /**
     * @brief updates the viewport to match the dimensions of the stored SDL_Window*
     */
    void resizeViewport();

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