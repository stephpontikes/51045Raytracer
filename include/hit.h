#ifndef HIT_H
#define HIT_H

#include <memory>

#include "geometries.h"
#include "materials.h"
#include "vector3.h"

using namespace mpcs51045;

namespace mpcs51045 {

using std::unique_ptr;

struct HitData {
    bool didHit;
    double distance;
    Vector3<double> hitPoint;
    Vector3<double> hitNormal;
};

HitData sphereIntersect(Ray const& ray, Sphere const& sphere) {
    HitData result;

    Vector3<double> offsetRayPos = ray.position - sphere.coordinates;
    Vector3<double> dir = ray.direction;
    dir.normalize();

    double a = Vector3<double>::dot(dir, dir);
    double b = 2.0 * Vector3<double>::dot(offsetRayPos, dir);
    double c = Vector3<double>::dot(offsetRayPos, offsetRayPos) -
               sphere.radius * sphere.radius;

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant > 0.0) {
        double discSqrt = sqrt(discriminant);
        double i1 = (-b + discSqrt) / (2.0 * a);
        double i2 = (-b - discSqrt) / (2.0 * a);

        if ((i1 < 0.0) || (i2 < 0.0)) {
            result.didHit = false;
            return result;
        }

        result.didHit = true;

        if (i1 < i2) {
            result.distance = i1;
        } else {
            result.distance = i2;
        }

        result.hitPoint = ray.position + (dir * result.distance);
        result.hitNormal = result.hitPoint - sphere.coordinates;
        result.hitNormal.normalize();
    } else {
        result.didHit = false;
        return result;
    }

    return result;
}

bool almostEqual(double const a, double const b) {
    return abs(a - b) < 1e-10;
}

std::ostream& operator<<(std::ostream& os, HitData hd) {
    os << "HitData(" << endl;
    os << "    didHit: " << hd.didHit << endl;
    os << "    distance: " << hd.distance << endl;
    os << "    hitPoint: " << hd.hitPoint << endl;
    os << "    hitNormal: " << hd.hitNormal << endl;
    os << ")" << endl;
    return os;
}

}  // namespace mpcs51045

#endif