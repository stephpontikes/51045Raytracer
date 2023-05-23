#include <iostream>
#include <memory>

#include "vector3.h"
// #include "camera.h"
// #include "graphics.h"
#include "factory.h"
#include "mesh.h"
// #include "ray.h"

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;

using namespace mpcs51045;

int main() {
    int windowWidth = 1280;
    int windowHeight = 720;

    // unique_ptr<Graphics> graphics = make_unique<Graphics>(windowWidth,
    //                                                       windowHeight);
    // graphics->run();

    unique_ptr<GlossyMeshFactory> gmf = make_unique<GlossyMeshFactory>();
    auto gs = gmf->create<Triangle>(Vector3<double>(30.0, 40.0, 50.0));

    unique_ptr<Mesh<Geometry, Material>> mesh = std::move(gs);
    std::cout << mesh->geometry->coordinates << std::endl;

    return 0;
}
