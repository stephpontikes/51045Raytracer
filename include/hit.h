#ifndef HIT_H
#define HIT_H

#include <memory>

#include "vector3.h"
#include "materials.h"

namespace mpcs51045 {

using std::unique_ptr;

struct HitData {
    bool didHit;
    double distance;
    Vector3<double> hitPoint;
    Vector3<double> hitNormal;
};

struct Sphere {
    Vector3<double> center;
    double radius;
    unique_ptr<Material> material;
};

HitData sphereIntersect(Ray const &ray, Sphere const &sphere) {
    HitData result;

    Vector3<double> offsetRayPos = ray.position - sphere.center;

    double a = Vector3<double>::dot(ray.direction, ray.direction);
    // double a = 1.0;
    double b = 2 * Vector3<double>::dot(offsetRayPos, ray.direction);
    double c = Vector3<double>::dot(offsetRayPos, offsetRayPos) - 
                sphere.radius * sphere.radius;
    // cout << a << " " << b << " " << c << endl;
    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        double i1 = (-b - sqrt(discriminant)) / (2 * a);
        double i2 = (-b + sqrt(discriminant)) / (2 * a);

        if (i1 < 0.0 || i2 < 0.0) {
            result.didHit = false;
            return result;
        }

        result.didHit = true;

        if (i1 < i2) {
            result.distance = i1;
        } else {
            result.distance = i2;
        }
        
        result.hitPoint = ray.position + ray.direction * result.distance;
        result.hitNormal = result.hitPoint - sphere.center;
        result.hitNormal.normalize();
    }

    return result;
}

bool almostEqual(double const a, double const b) {
    return abs(a - b) < 1e-10;
}

}  // namespace mpcs51045

#endif