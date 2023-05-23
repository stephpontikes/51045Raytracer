#ifndef HIT_H
#define HIT_H

#include <memory>

#include "geometries.h"
#include "materials.h"
#include "mesh.h"
#include "random.h"
#include "vector3.h"

using namespace mpcs51045;

namespace mpcs51045 {

using std::shared_ptr;
using std::unique_ptr;

struct HitData {
    bool didHit;
    double distance;
    Vector3<double> hitPoint;
    Vector3<double> hitNormal;
};

HitData sphereIntersect(Ray const& ray, unique_ptr<Geometry> const& geom) {
    HitData result;
    Sphere sphere = *dynamic_cast<Sphere*>(geom.get());

    Vector3<double> offsetRayPos = ray.position - sphere.coordinates;
    Vector3<double> dir = ray.direction;
    dir.normalize();

    // cout << "Coords: " << sphere.coordinates << ", Rad: " << sphere.radius << endl;
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

Vector3<double> handleHit(Ray& cameraRay,
                          unique_ptr<Mesh<Geometry, Material>>& current,
                          HitData const& hitData) {
    // Vector3<double> incomingLight{0.0, 0.0, 0.0};

    // cout << "Hit: " << intersectPoint << endl;
    // cout << "Ray Dir: " << cameraRay.direction << endl;
    // Move ray
    cameraRay.position = hitData.hitPoint;
    cameraRay.direction = randomReboundDirection(hitData.hitNormal) + hitData.hitNormal;
    cameraRay.direction.normalize();
    // cout << "New Direction: " << cameraRay.direction << endl;

    unique_ptr<Material> material = current->material->clone();
    Vector3<double> color = material->color();
    // cout << "Material Color: " << color << endl;
    // color /= 255.0;
    Vector3<double> emittedLight = color * material->luminosity();

    // cout << "Emitted Light: " << emittedLight << endl;
    // wrapRGBValues(emittedLight);
    // cout << "Updated Emitted Light: " << emittedLight << endl;
    // cout << "Luminosity: " << material->luminosity() << endl;
    // double lightStrength = Vector3<double>::dot(hitData.hitNormal, cameraRay.direction);
    // cout << "Light Strength: " << lightStrength << endl;
    Vector3<double> incomingLight = (emittedLight * cameraRay.color) / 255.0;
    // incomingLight *= 255.0;
    // cout << "Incoming Light: " << incomingLight << endl;
    // cout << "Old Ray Color: " << cameraRay.color << endl;
    cameraRay.color = (cameraRay.color * color) / 255.0;  // * lightStrength * 2.0;
    // cout << "New Ray Color: " << cameraRay.color << endl;

    return incomingLight;
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