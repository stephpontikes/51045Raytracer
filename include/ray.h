#ifndef RAY_H
#define RAY_H

#include <iostream>

#include "IndentStream.h"
#include "vector3.h"

namespace mpcs51045 {

using std::endl;
// bounce count, color, direction

class Ray {
   public:
    Vector3<double> position;
    Vector3<double> direction;
    Vector3<double> color;

    Ray() = default;
    Ray(Vector3<double> pos, Vector3<double> dir) : position(pos), direction(dir), color(255.0, 255.0, 255.0) {}
    Ray(Vector3<double> d, double bc, Vector3<double> c) : direction(d), color(c) {}
    Ray(const Ray&) = default;
    ~Ray() = default;
};

std::ostream& operator<<(std::ostream& os, const Ray& r) {
    mpcs51045::IndentStream is{os};
    is << "Ray(" << endl;
    is << mpcs51045::indent << "Position: " << r.position << endl;
    is << "Direction: " << r.direction << endl;
    is << "Color: " << r.color << endl;
    is << mpcs51045::unindent << ")";

    return os;
}

}  // namespace mpcs51045

#endif