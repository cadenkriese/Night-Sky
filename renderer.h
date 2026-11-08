#ifndef RENDERER_H
#define RENDERER_H

#define GL_GLEXT_PROTOTYPES

#include <vector>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>

#include "camera.h"
#include "renderable_star.h"

class Renderer {
public:
    Renderer(SDL_Window*);
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    /**
     * @brief renders a frame using OpenGL and swaps it to the stored SDL_Window*
     * 
     * @param STARS the stars to be drawn
     * @param CAMERA the camera representing the observer
     */
    void draw(const std::vector<RenderableStar>& STARS, const Camera& CAMERA);

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

    unsigned int compileShader(std::string, GLenum);
    void makeShaderProgram();
    void makeBuffers();
};
#endif // RENDERER_H