#include <iostream>
#include <memory>

#include "graphics.h"
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

    Graphics graphics = Graphics(1280, 720);
    graphics.run();

    return 0;
}
