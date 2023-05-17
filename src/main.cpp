#include <SDL3/SDL.h>

#include <iostream>
#include <memory>

#include "materials.h"

using std::cout;
using std::endl;
using std::unique_ptr;

void runWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not be initialized: " << SDL_GetError() << endl;
    } else {
        cout << "SDL is ready" << endl;
    }

    SDL_Window* window = SDL_CreateWindow("Press Q to Quit", 640, 480, SDL_WINDOW_OPENGL);

    bool isRunning = true;
    while (isRunning) {
        // glViewport(0, 0, 640, 480);

        SDL_Event event;
        // Start our event loop
        while (SDL_PollEvent(&event)) {
            // Handle each specific event
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            // Queries mouse motion
            // if (event.type == SDL_EVENT_MOUSE_MOTION) {
            //     cout << "mouse has been moved\n";
            // }
            // Queries key presses
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.keysym.sym == SDLK_q) {
                    cout << "q was pressed, quitting..." << endl;
                    isRunning = false;
                    break;
                }
            }

            // Queries arrow keys
            // const Uint8* state = SDL_GetKeyboardState(NULL);
            // if (state[SDL_SCANCODE_RIGHT]) {
            //     cout << "right arrow key is pressed\n";
            // }
        }

        // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        // glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    unique_ptr<AbstractMaterialFactory> glossyFactory = make_unique<GlossyMaterialFactory>();
    unique_ptr<Material> glossy(glossyFactory->create<Material>());

    unique_ptr<AbstractMaterialFactory> matteFactory = make_unique<MatteMaterialFactory>();
    unique_ptr<Material> matte(matteFactory->create<Material>());

    cout << "Glossy: " << glossy->reflectivity() << endl;
    cout << "Matte: " << matte->reflectivity() << endl;

    runWindow();

    return 0;
}
