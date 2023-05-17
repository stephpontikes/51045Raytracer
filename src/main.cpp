#include <SDL3/SDL.h>

#include <iostream>
#include <memory>

#include "materials.h"

using std::cout;
using std::endl;
using std::unique_ptr;

int main() {
    unique_ptr<AbstractMaterialFactory> glossyFactory = make_unique<GlossyMaterialFactory>();
    unique_ptr<Material> glossy(glossyFactory->create<Material>());

    unique_ptr<AbstractMaterialFactory> matteFactory = make_unique<MatteMaterialFactory>();
    unique_ptr<Material> matte(matteFactory->create<Material>());

    cout << "Glossy: " << glossy->reflectivity() << endl;
    cout << "Matte: " << matte->reflectivity() << endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not be initialized: " << SDL_GetError() << endl;
    } else {
        cout << "SDL is ready" << endl;
    }

    return 0;
}
