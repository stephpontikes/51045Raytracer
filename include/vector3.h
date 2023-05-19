#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

template <typename T>
concept Numeric = (std::floating_point<T> || std::integral<T>);

template <Numeric T>
struct Vector3 {
    Vector3() = default;
    Vector3(T const& _x, T const& _y, T const& _z) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

    Vector3(const Vector3& v1, const Vector3& v2) {
        x = v2.x - v1.x;
        y = v2.y - v1.y;
        z = v2.z - v1.z;
    }

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

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
    os << "<" << v.x << " " << v.y << " " << v.z << ">";
    return os;
}

template <std::integral T>
struct Vector3<T> {
    void normalize() = delete;
};

#endif