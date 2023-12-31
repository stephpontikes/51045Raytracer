#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include <vector>

#include "materials.h"
#include "opencl_struct.h"
#include "variadics/variadic_examples.h"
#include "vector3.h"

namespace mpcs51045 {
// Edited geometry class to be a base class containing only a coordinate
struct Geometry {
    Geometry() = default;
    Geometry(Geometry const& g) : coordinates(g.coordinates) {}
    Geometry(Vector3<double> const& v) : coordinates(v) {}

    virtual ~Geometry() = default;

    virtual std::unique_ptr<Geometry> clone() const = 0;

    Vector3<double> coordinates;
};

// Concrete geometries now inherit from Geometry base class

class Sphere : public Geometry {
   public:
    Sphere(Vector3<double> c, double r) : Geometry(c), radius(r) {}
    // temporary for testing, replace when static_cast removed
    Sphere(Geometry& g) : Geometry(g.coordinates), radius(1.0) {}

    ~Sphere() = default;

    virtual std::unique_ptr<Geometry> clone() const override {
        return std::make_unique<Sphere>(*this);
    }

    double radius;
};

// Triangles position translates all points of the triangle, removed material member (encapsulated in mesh)
class Triangle : public Geometry {
   public:
    Triangle() = default;

    Triangle(Vector3<double> const& pos, Vector3<double> _v1, Vector3<double> _v2, Vector3<double> _v3)
        : Geometry(pos), v1(_v1 + pos), v2(_v2 + pos), v3(_v3 + pos) {}

    ~Triangle() = default;

    virtual std::unique_ptr<Geometry> clone() const override {
        return std::make_unique<Triangle>(*this);
    }

    void setPosition(Vector3<double> const& pos) {
        v1 += pos;
        v2 += pos;
        v3 += pos;
    }

    Vector3<double> v1;
    Vector3<double> v2;
    Vector3<double> v3;
};

class ComplexGeometry : public Geometry {
   public:
    ComplexGeometry() = delete;
    ComplexGeometry(Vector3<double> const& pos, std::vector<Triangle>& triangle_vec)
        : Geometry(pos) {
        for (auto& t : triangle_vec) {
            t.setPosition(pos);
        }
        triangles = triangle_vec;
    }

    std::vector<Triangle> triangles;
};

template <typename... Ts>
class MultipleGeometry : public Ts... {
   public:
    // use overloading/type tags in base classes to get proper onhit behavior for each geometry
    std::vector<std::unique_ptr<Geometry>> geometries;
};

// using AbstractGeometryFactory = mpcs51045::abstract_factory<Geometry>;
// using GeometryFactory = mpcs51045::concrete_factory<AbstractGeometryFactory,
//                                                     Sphere, Triangle>;

SphereCL toCL(Sphere& geom, Material& mat) {
    return SphereCL{
        toCL(geom.coordinates), static_cast<cl_float>(geom.radius),
        toCL(mat.color()),
        static_cast<cl_float>(mat.reflectivity()),
        static_cast<cl_float>(mat.luminosity()),
        static_cast<cl_float>(mat.smoothness())};
}

TriangleCL toCL(Triangle& geom) {
    return TriangleCL{toCL(geom.coordinates), toCL(geom.v1), toCL(geom.v2), toCL(geom.v3)};
}

using geometry_types = typelist<Geometry, Sphere, Triangle>;

}  // namespace mpcs51045

#endif