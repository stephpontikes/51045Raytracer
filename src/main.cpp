#include <assert.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "camera.h"
#include "factory.h"
#include "graphics.h"
#include "mesh.h"
#include "opencl_struct.h"
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

    auto start = high_resolution_clock::now();
    unique_ptr<Graphics> graphics = make_unique<Graphics>(windowWidth,
                                                          windowHeight);

    cout << "Render Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << "ms" << endl;

    graphics->run();

    return 0;
}
