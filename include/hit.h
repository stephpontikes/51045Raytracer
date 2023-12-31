#ifndef HIT_H
#define HIT_H

#include <memory>

#include "geometries.h"
#include "materials.h"
#include "mesh.h"
#include "opencl_struct.h"
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
    result.didHit = false;
    Sphere* sphere = dynamic_cast<Sphere*>(geom.get());
    Vector3<double> offsetRayPos = ray.position - sphere->coordinates;
    Vector3<double> dir = ray.direction;
    dir.normalize();

    double a = Vector3<double>::dot(dir, dir);
    double b = 2.0 * Vector3<double>::dot(offsetRayPos, dir);
    double c = Vector3<double>::dot(offsetRayPos, offsetRayPos) -
               sphere->radius * sphere->radius;

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
        result.hitNormal = result.hitPoint - sphere->coordinates;
        result.hitNormal.normalize();
    } else {
        result.didHit = false;
    }

    return result;
}

std::pair<HitData, int> getClosestHit(Ray& cameraRay,
                                      std::vector<unique_ptr<Mesh<Geometry, Material>>> const& objects) {
    HitData closestHit;
    HitData currentHit;
    int closestIndex = -1;
    closestHit.distance = INFINITY;
    // Test for intersections with all objects (replace with visitor in future)
    for (int i = 0; i < objects.size(); i++) {
        auto& current = objects.at(i);
        currentHit = sphereIntersect(cameraRay, current->geometry);

        if (currentHit.didHit && currentHit.distance < closestHit.distance) {
            closestHit.didHit = currentHit.didHit;
            closestHit.distance = currentHit.distance;
            closestHit.hitPoint = currentHit.hitPoint;
            closestHit.hitNormal = currentHit.hitNormal;
            closestIndex = i;
        }
    }

    return std::make_pair(closestHit, closestIndex);
}

Vector3<double> handleHit(Ray& cameraRay,
                          unique_ptr<Material> const& material,
                          HitData const& hitData) {
    Vector3<double> color = material->color();

    // Move ray
    cameraRay.position = hitData.hitPoint;
    Vector3<double> diffuseDir = randomReboundDirection(hitData.hitNormal) + hitData.hitNormal;
    diffuseDir.normalize();
    Vector3<double> specularDir = Vector3<double>::reflect(cameraRay.direction, hitData.hitNormal);
    bool isSpecular = material->reflectivity() >= getRandomProbValue();
    cameraRay.direction = Vector3<double>::interpolate(diffuseDir, specularDir, material->smoothness() * isSpecular);
    cameraRay.direction.normalize();
    Vector3<double> emittedLight = color * material->luminosity();

    Vector3<double> incomingLight = (emittedLight * cameraRay.color) / 255.0;
    Vector3<double> reflectivityColor = Vector3<double>::interpolate(color, Vector3<double>(255.0, 255.0, 255.0), isSpecular);
    cameraRay.color = (cameraRay.color * reflectivityColor) / 255.0;

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

HitDataCL toCL(HitData& hitData) {
    auto point = hitData.hitPoint;
    auto norm = hitData.hitNormal;

    HitDataCL result = {hitData.didHit, static_cast<cl_float>(hitData.distance),
                        toCL(hitData.hitPoint), toCL(hitData.hitNormal)};
    return result;
}

}  // namespace mpcs51045

#endif