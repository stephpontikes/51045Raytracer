#include <iostream>
#include <memory>

#include "camera.h"
#include "graphics.h"
#include "materials.h"
#include "ray.h"

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;

int main() {
    int windowWidth = 1280;
    int windowHeight = 720;

    unique_ptr<Graphics> graphics = make_unique<Graphics>(windowWidth,
                                                          windowHeight);
    graphics->run();

    return 0;
}
