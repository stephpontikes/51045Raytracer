#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

template <typename T>
concept Numeric = (std::floating_point<T> || std::integral<T>);

template <Numeric T>
struct Vector3 {
    Vector3() = default;
    Vector3(T const& _x, T const& _y, T const& _z) : x(_x), y(_y), z(_z) {}
    Vector3(Vector3& v) : x(v.x), y(v.y), z(v.z) {}

    ~Vector3() = default;

    double norm() const { return sqrt(x * x + y * y + z * z); }

    void normalize() {
        double dist = norm();
        x /= dist;
        y /= dist;
        z /= dist;
    }

    T x = 0;
    T y = 0;
    T z = 0;
};

template <std::integral T>
struct Vector3Int : Vector3<T> {
    void normalize() = delete;
};

#endif