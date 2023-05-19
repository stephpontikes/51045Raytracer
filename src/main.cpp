#include <iostream>
#include <memory>

#include "graphics.h"
#include "materials.h"
#include "ray.h"

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;

class Camera {
   public:
    Camera(double _x, double _y, double _z) : position(Vector3<double>{_x, _y, _z}) {}
    Vector3<double> position;
};

int main() {
    int windowWidth = 1280;
    int windowHeight = 720;
    unique_ptr<Camera> camera = make_unique<Camera>(windowWidth / 2.0,
                                                    windowHeight / 2.0,
                                                    25.0);
    vector<Ray> initRays{};
    for (int x = 0; x < windowWidth; x++) {
        for (int y = 0; y < windowHeight; y++) {
            Vector3<double> pixel{double(x), double(y), 0.0};
            Vector3<double> diff{Vector3<double>{pixel, camera->position}};
            // cout << diff << endl;
            Ray r{diff};
            cout << r << endl;
            initRays.push_back(r);
            break;
        }
        break;
    }
    // Ray r1{Vector3<double>(0.0, 0.0, 0.0)};
    // Ray r2{Vector3<double>(1.0, 2.0, 3.0)};
    // Vector3<double> v{r1.direction, r2.direction};
    // initRays.push_back(r);
    // cout << initRays.at(0) << endl;
    // cout << v << endl;

    // unique_ptr<Graphics> graphics = make_unique<Graphics>(windowWidth,
    //                                                       windowHeight);
    // graphics->run();

    return 0;
}
