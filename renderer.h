#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <SDL3/SDL_video.h>

#include "renderable_star.h"

class Renderer {
public:
    Renderer(SDL_Window*);
    ~Renderer();
    
    /**
     * @brief renders a frame using OpenGL and swaps it to the stored SDL_Window*
     * 
     * @param STARS the stars to be drawn
     */
    void draw(const std::vector<RenderableStar>& STARS);

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

    void compileShaders();
    void makeBuffers();
};
#endif // RENDERER_H