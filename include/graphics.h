#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>

#include <iostream>
#include <memory>

using std::cout;
using std::endl;

namespace mpcs51045 {

class Graphics {
   public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int width;
    int height;
    bool isRunning = false;

    Graphics(int w, int h) : width(w), height(h) {}

    void run() {
        init();

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

    void init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cout << "SDL could not be initialized: " << SDL_GetError() << endl;
        } else {
            cout << "SDL is ready" << endl;
        }

        window = SDL_CreateWindow("Press Q to Quit", width, height, 0);

        if (window != NULL) {
            renderer = SDL_CreateRenderer(window, NULL, 0);
        }
    }

    void handleEvent(SDL_Event* event) {
        // Handle each specific event
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    void quit() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        window = NULL;
        SDL_Quit();
    }
};
}  // namespace mpcs51045

#endif