#include <iostream>
#include <memory>

#include "camera.h"
// #include "graphics.h"
#include "materials.h"
#include "ray.h"

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;

int main() {
    int windowWidth = 1280;
    int windowHeight = 720;
    // int depth = 720;
    // unique_ptr<Camera> camera = make_unique<Camera>(0.0, 0.0, 5.0);
    // Vector3<double> lookAt{0, 0, depth};
    // vector<Ray> initRays{};
    // for (int x = 0; x < windowWidth; x++) {
    //     for (int y = 0; y < windowHeight; y++) {
    //         Vector3<double> pixel{double(x), double(y), 0.0};
    //         Vector3<double> diff{Vector3<double>{pixel, camera->getPosition()}};
    //         // cout << diff << endl;
    //         Ray r{diff, Vector3<double>{0.0, 0.0, 0.0}};
    //         cout << r << endl;
    //         initRays.push_back(r);
    //         break;
    //     }
    //     break;
    // }

    // unique_ptr<Graphics> graphics = make_unique<Graphics>(windowWidth,
    //                                                       windowHeight);
    // graphics->run();

    return 0;
}
