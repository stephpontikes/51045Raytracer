#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include <vector>

#include "materials.h"
#include "vector3.h"

namespace mpcs51045 {
// Edited geometry class to be a base class containing only a coordinate
struct Geometry {
    Geometry() = default;
    Geometry(Geometry const& g) : coordinates(g.coordinates) {}
    Geometry(Vector3<double> const& v) : coordinates(v) {}

    virtual ~Geometry() = default;

    Vector3<double> coordinates;
};

// Concrete geometries now inherit from Geometry base class

// removed material data member
class Sphere : public Geometry {
   public:
    Sphere(Vector3<double> c, double r) : Geometry(c), radius(r) {}

    ~Sphere() = default;

    double radius;
};

// Triangles position translates all points of the triangle, removed material member (encapsulated in mesh)
class Triangle : public Geometry {
   public:
    Triangle() = default;

    Triangle(Vector3<double> const& pos, Vector3<double> _v1, Vector3<double> _v2, Vector3<double> _v3, Material& m)
        : Geometry(pos), v1(_v1 + pos), v2(_v2 + pos), v3(_v3 + pos) {}

    ~Triangle() = default;

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

// Visit method calls visit method on each geometry in mesh
template <typename... Ts>
class MultipleGeometry : public Ts... {
   public:
    // use overloading/type tags in base classes to get proper onhit behavior for each geometry
    // discuss w logan, possibly better design philosophies exist. but this uses advanced variadics
    std::vector<unique_ptr<Geometry>> geometries;
};
}  // namespace mpcs51045

#endif