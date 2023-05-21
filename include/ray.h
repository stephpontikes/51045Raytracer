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
    double bounce_count = 0;
    Vector3<double> color;

    Ray() = default;
    Ray(Vector3<double> pos, Vector3<double> dir) : position(pos), direction(dir), bounce_count(0), color(255.0, 255.0, 255.0) {}
    Ray(Vector3<double> d, double bc, Vector3<double> c) : direction(d), bounce_count(bc), color(c) {}
    Ray(const Ray&) = default;
    ~Ray() = default;

    void bounce(Vector3<double>& new_direction, Vector3<double> new_color) {
        bounce_count++;
        direction = new_direction;

        color.x = (new_color.x + (bounce_count * color.x)) / (1 + bounce_count);
        color.y = (new_color.y + (bounce_count * color.y)) / (1 + bounce_count);
        color.z = (new_color.z + (bounce_count * color.z)) / (1 + bounce_count);
    }
};

std::ostream& operator<<(std::ostream& os, const Ray& r) {
    mpcs51045::IndentStream is{os};
    is << "Ray(" << endl;
    is << mpcs51045::indent << "Position: " << r.position << endl;
    is << "Direction: " << r.direction << endl;
    is << "Bounce Count: " << r.bounce_count << endl;
    is << "Color: " << r.color << endl;
    is << mpcs51045::unindent << ")";

    return os;
}

}  // namespace mpcs51045

#endif