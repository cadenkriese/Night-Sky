#include <iostream>

#include <SDL3/SDL.h>

#include "input_helper.h"
#include "renderer.h"

using namespace std;

void makeWindowWithOpenGL(SDL_Window*& window, SDL_GLContext& glContext) {
    // Set OpenGL version to 3.3 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Handle scaling for high resolution displays
    float displayScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    // Create a window with SDL
    SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    window = SDL_CreateWindow("Night Sky", (int)(1280 * displayScale), (int)(720 * displayScale), windowFlags);
    if (window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        // TODO exception
    }
    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        // TODO exception
    }

    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);
}

int main() {
    // Set up SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_GLContext glContext;
    makeWindowWithOpenGL(window, glContext);
    // TODO handle exceptions

    // State
    InputHelper ioHelper; // maybe rename to star catalog and add a filter method?
    vector<Star> stars = ioHelper.loadStarsFromCSV("gaiadr3.csv");

    vector<float> vertices = {
        -0.5f, -0.5f, 0.f,
         0.5f, -0.5f, 0.f,
         0.0f,  0.5f, 0.f,
    };
    Renderer renderer(window, vertices);
    
    // Draw loop
    bool done = false;
    while (!done) {
        // Input handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)) {
                done = true;
            }
            // Update OpenGL viewport dimensions when a resize occurs
            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED && event.window.windowID == SDL_GetWindowID(window)) {
                renderer.resizeViewport();
            }
        }

        // Skip drawing when the window is minimized
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

        // Draw
        renderer.draw();
    }

    // Cleanup
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
