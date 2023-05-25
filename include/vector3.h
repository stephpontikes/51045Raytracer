#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

namespace mpcs51045 {

// template <typename T>
// concept Numeric = (std::floating_point<T> || std::integral<T>);

// template <Numeric T>
// struct Vector3 {
//     Vector3() = default;
//     Vector3(T const& _x, T const& _y, T const& _z) : x(_x), y(_y), z(_z) {}
//     Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

//     Vector3(const Vector3& v1, const Vector3& v2) {
//         x = v2.x - v1.x;
//         y = v2.y - v1.y;
//         z = v2.z - v1.z;
//     }

//     Vector3& operator=(const Vector3&) = default;

//     ~Vector3() = default;

//     double norm() const { return sqrt(x * x + y * y + z * z); }

//     void normalize() {
//         double dist = norm();
//         x /= dist;
//         y /= dist;
//         z /= dist;
//     }

//     static T dot(Vector3<T> const& u, Vector3<T> const& v) {
//         return u.x * v.x + u.y * v.y + u.z * v.z;
//     }

//     static Vector3<T> cross(Vector3<T> const& u, Vector3<T> const& v) {
//         Vector3<T> res{(u.y * v.z) - (u.z * v.y),
//                        -((u.x * v.z) - (u.z * v.x)),
//                        (u.x * v.y) - (u.y * v.x)};

//         return res;
//     }

//     Vector3<T>& operator-=(Vector3<T> const& rhs) {
//         this->x = this->x - rhs.x;
//         this->y = this->y - rhs.y;
//         this->z = this->z - rhs.z;

//         return *this;
//     }

//     friend Vector3<T> operator-(Vector3<T> lhs, Vector3<T> const& rhs) {
//         lhs -= rhs;
//         return lhs;
//     }

//     Vector3<T>& operator+=(const Vector3<T>& rhs) {
//         this->x = this->x + rhs.x;
//         this->y = this->y + rhs.y;
//         this->z = this->z + rhs.z;

//         return *this;
//     }

//     friend Vector3<T> operator+(Vector3<T> lhs, Vector3<T> const& rhs) {
//         lhs += rhs;
//         return lhs;
//     }

//     Vector3<T>& operator*=(Vector3<T> const& rhs) {
//         this->x = this->x * rhs.x;
//         this->y = this->y * rhs.y;
//         this->z = this->z * rhs.z;

//         return *this;
//     }

//     Vector3<T>& operator*=(const T& val) {
//         this->x = this->x * val;
//         this->y = this->y * val;
//         this->z = this->z * val;

//         return *this;
//     }

//     friend Vector3<T> operator*(Numeric auto scalar, Vector3<T> const& rhs) {
//         return Vector3<T>{rhs.x * scalar, rhs.y * scalar, rhs.z * scalar};
//     }

//     friend Vector3<T> operator*(Vector3<T> const& lhs, Numeric auto scalar) {
//         return scalar * lhs;
//     }

//     friend Vector3<T> operator*(Vector3<T> const& lhs, Vector3<T> const& rhs) {
//         return Vector3<T>{lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
//     }

//     Vector3<T>& operator/=(const T& val) {
//         this->x = this->x / val;
//         this->y = this->y / val;
//         this->z = this->z / val;

//         return *this;
//     }

//     friend Vector3<T> operator/(Vector3<T> const& lhs, Numeric auto scalar) {
//         return Vector3<T>{lhs.x / scalar, lhs.y / scalar, lhs.z / scalar};
//     }

//     T x = 0;
//     T y = 0;
//     T z = 0;
// };

// template <Numeric T>
// Vector3<T> operator-(Vector3<T> lhs, Vector3<T> const& rhs);

// template <Numeric T>
// Vector3<T> operator+(Vector3<T> lhs, Vector3<T> const& rhs);

// template <Numeric T>
// Vector3<T> operator*(Numeric auto scalar, Vector3<T> const& rhs);

// template <Numeric T>
// Vector3<T> operator*(Vector3<T> const& lhs, Numeric auto scalar);

// template <Numeric T>
// Vector3<T> operator*(Vector3<T> const& lhs, Vector3<T> const& rhs);

// template <Numeric T>
// Vector3<T> operator/(Vector3<T> const& lhs, Numeric auto scalar);

// template <Numeric T>
// std::ostream& operator<<(std::ostream& os, Vector3<T> const& v) {
//     os << "<" << v.x << " " << v.y << " " << v.z << ">";
//     return os;
// }

// template <std::integral T>
// struct Vector3<T> {
//     void normalize() = delete;
// };

// template <typename T>
// concept Numeric = (std::floating_point<T> || std::integral<T>);

template <typename T>
struct Vector3 {
    Vector3() = default;
    Vector3(T const& _x, T const& _y, T const& _z) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

    Vector3(const Vector3& v1, const Vector3& v2) {
        x = v2.x - v1.x;
        y = v2.y - v1.y;
        z = v2.z - v1.z;
    }

    Vector3& operator=(const Vector3&) = default;

    ~Vector3() = default;

    double norm() const { return sqrt(x * x + y * y + z * z); }

    void normalize() {
        double dist = norm();
        x /= dist;
        y /= dist;
        z /= dist;
    }

    static T dot(Vector3<T> const& u, Vector3<T> const& v) {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    static Vector3<T> cross(Vector3<T> const& u, Vector3<T> const& v) {
        Vector3<T> res{(u.y * v.z) - (u.z * v.y),
                       -((u.x * v.z) - (u.z * v.x)),
                       (u.x * v.y) - (u.y * v.x)};

        return res;
    }

    static Vector3<T> reflect(Vector3<T> const& dir, Vector3<T> const& normal) {
        return dir - 2.0 * Vector3<T>::dot(dir, normal) * normal;
    }

    static T interpolate(T const& from, T const& to, double factor) {
        return (1.0 - factor) * from + factor * to;
    }

    static Vector3<T> interpolate(Vector3<T> const& from, Vector3<T> const& to, double factor) {
        return Vector3<T>(interpolate(from.x, to.x, factor), interpolate(from.y, to.y, factor), interpolate(from.z, to.z, factor));
    }

    Vector3<T>&
    operator-=(Vector3<T> const& rhs) {
        this->x = this->x - rhs.x;
        this->y = this->y - rhs.y;
        this->z = this->z - rhs.z;

        return *this;
    }

    friend Vector3<T> operator-(Vector3<T> lhs, Vector3<T> const& rhs) {
        lhs -= rhs;
        return lhs;
    }

    Vector3<T>& operator+=(const Vector3<T>& rhs) {
        this->x = this->x + rhs.x;
        this->y = this->y + rhs.y;
        this->z = this->z + rhs.z;

        return *this;
    }

    friend Vector3<T> operator+(Vector3<T> lhs, Vector3<T> const& rhs) {
        lhs += rhs;
        return lhs;
    }

    Vector3<T>& operator*=(Vector3<T> const& rhs) {
        this->x = this->x * rhs.x;
        this->y = this->y * rhs.y;
        this->z = this->z * rhs.z;

        return *this;
    }

    Vector3<T>& operator*=(const T& val) {
        this->x = this->x * val;
        this->y = this->y * val;
        this->z = this->z * val;

        return *this;
    }

    friend Vector3<T> operator*(auto scalar, Vector3<T> const& rhs) {
        return Vector3<T>{rhs.x * scalar, rhs.y * scalar, rhs.z * scalar};
    }

    friend Vector3<T> operator*(Vector3<T> const& lhs, auto scalar) {
        return scalar * lhs;
    }

    friend Vector3<T> operator*(Vector3<T> const& lhs, Vector3<T> const& rhs) {
        return Vector3<T>{lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
    }

    Vector3<T>& operator/=(const T& val) {
        this->x = this->x / val;
        this->y = this->y / val;
        this->z = this->z / val;

        return *this;
    }

    friend Vector3<T> operator/(Vector3<T> const& lhs, auto scalar) {
        return Vector3<T>{lhs.x / scalar, lhs.y / scalar, lhs.z / scalar};
    }

    T x = 0;
    T y = 0;
    T z = 0;
};

template <typename T>
Vector3<T> operator-(Vector3<T> lhs, Vector3<T> const& rhs);

template <typename T>
Vector3<T> operator+(Vector3<T> lhs, Vector3<T> const& rhs);

template <typename T>
Vector3<T> operator*(auto scalar, Vector3<T> const& rhs);

template <typename T>
Vector3<T> operator*(Vector3<T> const& lhs, auto scalar);

template <typename T>
Vector3<T> operator*(Vector3<T> const& lhs, Vector3<T> const& rhs);

template <typename T>
Vector3<T> operator/(Vector3<T> const& lhs, auto scalar);

template <typename T>
std::ostream& operator<<(std::ostream& os, Vector3<T> const& v) {
    os << "<" << v.x << " " << v.y << " " << v.z << ">";
    return os;
}

template <typename T>
inline bool operator==(Vector3<T> const& lhs, Vector3<T> const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename T>
inline bool operator!=(Vector3<T> const& lhs, Vector3<T> const& rhs) {
    return !(lhs == rhs);
}

}  // namespace mpcs51045

#endif