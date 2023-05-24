#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <memory>

#include "image.h"
#include "scene.h"

using std::cout;
using std::endl;

namespace mpcs51045 {

class Graphics {
   public:
    Graphics(int w, int h) {
        width = w;
        height = h;
        isRunning = false;
        window = nullptr;
        renderer = nullptr;
        glContext = nullptr;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cout << "SDL could not be initialized: " << SDL_GetError() << endl;
            return;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        window = SDL_CreateWindow("Press Q to Quit", width, height, SDL_WINDOW_OPENGL);
        int wdth, hght;
        SDL_GetWindowSizeInPixels(window, &wdth, &hght);

        if (window == nullptr) {
            return;
        }

        glContext = SDL_GL_CreateContext(window);

        if (glContext == nullptr) {
            return;
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            return;
        } else {
            cout << "----------OpenGL Info----------" << endl;
            cout << glGetString(GL_VENDOR) << endl;
            cout << glGetString(GL_RENDERER) << endl;
            cout << glGetString(GL_VERSION) << endl;
            cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
            cout << "-------------------------------" << endl;
        }

        renderer = SDL_CreateRenderer(window, NULL, 0);
        // Fixes high DPI issue with Mac Retina displays
        SDL_SetRenderLogicalPresentation(renderer, wdth, hght,
                                         SDL_LOGICAL_PRESENTATION_MATCH,
                                         SDL_SCALEMODE_NEAREST);

        image.init(renderer, width, height);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        scene.render(image);

        image.display();

        SDL_RenderPresent(renderer);
    }

    void run() {
        SDL_Event event;
        isRunning = true;

        while (isRunning) {
            while (SDL_PollEvent(&event)) {
                handleEvent(&event);
            }
            render();
        }

        quit();
    }

    void handleEvent(SDL_Event* event) {
        if (event->type == SDL_EVENT_QUIT) {
            isRunning = false;
        }
        // Queries mouse motion
        // if (event.type == SDL_EVENT_MOUSE_MOTION) {
        //     cout << "mouse has been moved\n";
        // }

        // Queries key presses
        if (event->type == SDL_EVENT_KEY_DOWN) {
            if (event->key.keysym.sym == SDLK_q) {
                cout << "q was pressed, quitting..." << endl;
                isRunning = false;
            }
        }

        // Queries arrow keys
        // const Uint8* state = SDL_GetKeyboardState(NULL);
        // if (state[SDL_SCANCODE_RIGHT]) {
        //     cout << "right arrow key is pressed\n";
        // }
    }

    void render() {
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // SDL_RenderClear(renderer);

        // scene.render(image);

        // image.display();

        // SDL_RenderPresent(renderer);
        // SDL_GL_SwapWindow(window);
    }

    void quit() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        window = NULL;
        SDL_Quit();
    }

   private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_GLContext glContext = nullptr;
    int width;
    int height;
    bool isRunning = false;
    Image image;
    Scene scene;
};

}  // namespace mpcs51045

#endif