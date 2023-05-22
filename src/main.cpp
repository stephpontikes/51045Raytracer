#include <chrono>
#include <iostream>
#include <memory>

#include "camera.h"
#include "factory.h"
#include "graphics.h"
#include "mesh.h"
#include "ray.h"
#include "vector3.h"

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;

using namespace mpcs51045;
using namespace std::chrono;

int main() {
    int windowWidth = 1280;
    int windowHeight = 720;

    // unique_ptr<GlossyMeshFactory> gmf = make_unique<GlossyMeshFactory>();
    // auto gs = gmf->create<Triangle>(Vector3<double>(255.0, 255.0, 255.0), Vector3<double>(0.0, 5.0, 0.0), Vector3<double>(0.0, 5.0, 0.0), Vector3<double>(0.0, 5.0, 0.0), Vector3<double>(0.0, 5.0, 0.0));
    // std::cout << gs->geometry->coordinates << std::endl;
    // std::cout << gs->material->color() << std::endl;

    auto start = high_resolution_clock::now();
    unique_ptr<Graphics> graphics = make_unique<Graphics>(windowWidth,
                                                          windowHeight);
    cout << "Render Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << "ms" << endl;

    graphics->run();

    return 0;
}
