#include <iostream>

#include <SDL3/SDL.h>

#include "input_helper.h"
#include "renderer.h"
#include "camera.h"

using namespace std;

const float MOUSE_SENSITIVITY = 0.005f;

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

    // Prompt for input
    InputHelper ioHelper;
    Location location = ioHelper.promptForLocation();
    chrono::system_clock::time_point time = ioHelper.promptForTime();
    vector<Star> stars = ioHelper.loadStarsFromCSV("gaiadr3.csv");

    // Convert stars to renderable ones, filter for stars above the horizon (altitude of 0º)
    vector<RenderableStar> visibleStars;
    
    for (size_t i = 0; i < stars.size(); i++) {
        RenderableStar renderableStar = stars.at(i).getRenderableStar(location, time);
        if (renderableStar.getAltitude() > 0) {
            visibleStars.push_back(renderableStar);
        }
    }

    // Make a window
    SDL_Window* window;
    SDL_GLContext glContext;
    makeWindowWithOpenGL(window, glContext);
    // TODO handle exceptions

    Renderer renderer(window);
    Camera camera;
    bool relativeMouseMode = false; // Is the mouse captured and being used to control the camera
    
    // Draw loop
    bool done = false;
    while (!done) {
        // Input handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                done = true;
            }
            // Update OpenGL viewport dimensions when a resize occurs
            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                renderer.resizeViewport();
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                relativeMouseMode = !relativeMouseMode;
                SDL_SetWindowRelativeMouseMode(window, relativeMouseMode);
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                relativeMouseMode = false;
                SDL_SetWindowRelativeMouseMode(window, relativeMouseMode);
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION && relativeMouseMode) {
                camera.setYaw(camera.getYaw() + MOUSE_SENSITIVITY * event.motion.xrel);
                camera.setPitch(camera.getPitch() - MOUSE_SENSITIVITY * event.motion.yrel);
            }
        }

        // Skip drawing when the window is minimized
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

        // Draw
        renderer.draw(visibleStars, camera);
    }

    // Cleanup
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
