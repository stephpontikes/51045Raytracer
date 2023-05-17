#include "materials.h"
#include "vector3.h"

struct Geometry {
    Vector3<double> coordinates;
};

class Sphere {
   public:
    Sphere(Vector3<double> c, double r, Material& m) : center(c), radius(r), mat(m) {}

    Vector3<double> center;
    double radius;
    Material& mat;
};

class Triangle {
   public:
    Triangle(Vector3<double> _v1, Vector3<double> _v2, Vector3<double> _v3, Material& m)
        : v1(_v1), v2(_v2), v3(_v3), mat(m) {}

    Vector3<double> v1;
    Vector3<double> v2;
    Vector3<double> v3;
    Material mat;
};