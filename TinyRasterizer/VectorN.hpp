#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>

namespace Rasterizer
{

    template <typename Number>
    class Vector3
    {
    public:
        union
        {
            struct
            {
                Number x, y, z;
            };
            struct
            {
                Number ivert, iuv, inorm;
            };
            Number raw[3];
        };
        Vector3() : x(0), y(0), z(0) {}
        Vector3(Number tt) : x(tt), y(tt), z(tt) {}
        Vector3(Number xx, Number yy, Number zz) : x(xx), y(yy), z(zz) {}
        inline Vector3<Number> operator^(const Vector3<Number> &v) const { return Vector3<Number>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
        inline Vector3<Number> operator+(const Vector3<Number> &v) const { return Vector3<Number>(x + v.x, y + v.y, z + v.z); }
        inline Vector3<Number> operator-(const Vector3<Number> &v) const { return Vector3<Number>(x - v.x, y - v.y, z - v.z); }
        inline Vector3<Number> operator/(const Vector3<Number> &v) const { return Vector3<Number>(x / v.x, y / v.y, z / v.z); }
        inline Vector3<Number> operator*(float f) const { return Vector3<Number>(x * f, y * f, z * f); }
        inline Number operator*(const Vector3<Number> &v) const { return x * v.x + y * v.y + z * v.z; }
        Number norm() const { return std::sqrt(x * x + y * y + z * z); }
        Vector3<Number> normalized() const
        {
            if (x == y && y == z && z == 0)
            {
                return (*this);
            }

            return (*this) * 1 / norm();
        }
        Vector3<Number> &normalize(Number l = 1)
        {
            if (x == y && y == z && z == 0)
            {
                return (*this);
            }
            *this = (*this) * (l / norm());
            return *this;
        }
        inline friend std::ostream &operator<<(std::ostream &s, Vector3<Number> &v)
        {
            s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
            return s;
        }
    };

    template <typename Number>
    class Vector2
    {
    public:
        union
        {
            struct
            {
                Number u, v;
            };
            struct
            {
                Number x, y;
            };
            Number raw[2];
        };
        Vector2() : x(0), y(0) {}
        Vector2(Number tt) : x(tt), y(tt) {}
        Vector2(Number xx, Number yy) : u(xx), v(yy) {}
        inline Vector2<Number> operator+(const Vector2<Number> &other) const { return Vector2<Number>(x + other.x, y + other.y); }
        inline Vector2<Number> operator-(const Vector2<Number> &other) const { return Vector2<Number>(x - other.x, y - other.y); }
        inline Vector2<Number> operator*(double f) const { return Vector2<Number>(x * f, y * f); }
        inline Number operator^(const Vector2<Number> &other) const { return Number(x * other.y - y * other.x); }

        inline friend std::ostream &operator<<(std::ostream &s, Vector2<Number> &v)
        {
            s << "(" << v.x << ", " << v.y << ")\n";
            return s;
        }
    };
    using Vector2D = Vector2<double>;
    using Vector2F = Vector2<float>;
    using Vector2I = Vector2<int>;
    using Vector3F = Vector3<float>;
    using Vector3D = Vector3<double>;
    using Vector3I = Vector3<int>;

    // template <typename Number>
}