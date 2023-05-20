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

    static Vector3<T> cross(const Vector3<T>& u, const Vector3<T>& v) {
        Vector3<T> res{(u.y * v.z) - (u.z * v.y),
                       -((u.x * v.z) - (u.z * v.x)),
                       (u.x * v.y) - (u.y * v.x)};

        return res;
    }

    Vector3<T>& operator-=(const Vector3<T>& rhs) {
        this->x = this->x - rhs.x;  // This is the easy one
        this->y = this->y - rhs.y;
        this->z = this->z - rhs.z;

        return *this;
    }

    friend Vector3<T> operator-(Vector3<T> lhs, const Vector3<T>& rhs) {
        lhs -= rhs;
        return lhs;
    }

    Vector3<T>& operator+=(const Vector3<T>& rhs) {
        this->x = this->x + rhs.x;  // This is the easy one
        this->y = this->y + rhs.y;
        this->z = this->z + rhs.z;

        return *this;
    }

    friend Vector3<T> operator+(Vector3<T> lhs, const Vector3<T>& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Vector3<T> operator*(Numeric auto scalar, const Vector3<T>& rhs) {
        return Vector3<T>{rhs.x * scalar, rhs.y * scalar, rhs.z * scalar};
    }

    friend Vector3<T> operator*(const Vector3<T>& lhs, Numeric auto scalar) {
        return scalar * lhs;
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