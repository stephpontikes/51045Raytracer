#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>

#include <iostream>
#include <memory>

#include "image.h"

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

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            cout << "SDL could not be initialized: " << SDL_GetError() << endl;
        } else {
            cout << "SDL is ready" << endl;
        }

        window = SDL_CreateWindow("Press Q to Quit", width, height, 0);

        if (window == NULL) {
            return;
        }
        renderer = SDL_CreateRenderer(window, NULL, 0);

        image.init(renderer, width, height);

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                double red = (static_cast<double>(x) / double(width)) * 255.0;
                double green = (static_cast<double>(y) / double(height)) * 255.0;
                image.setPixel(x, y, red, green, 0.0);
            }
        }
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        image.display();

        SDL_RenderPresent(renderer);
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
    int width;
    int height;
    bool isRunning = false;
    Image image;
};
}  // namespace mpcs51045

#endif