#pragma once
#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cmath>
namespace Rasterizer
{
    template <typename T>
    class Vector3
    {
    public:
        Vector3() : x(0), y(0), z(0) {}
        Vector3(T one) : x(one), y(one), z(one) {}
        Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
        Vector3 operator*(const T &r) const { return Vector3(x * r, y * r, z * r); }
        Vector3 operator/(const T &r) const { return Vector3(x / r, y / r, z / r); }

        Vector3 operator*(const Vector3 &v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
        Vector3 operator-(const Vector3 &v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
        Vector3 operator+(const Vector3 &v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
        Vector3 operator-() const { return Vector3(-x, -y, -z); }
        Vector3 &operator+=(const Vector3 &v)
        {
            x += v.x, y += v.y, z += v.z;
            return *this;
        }
        friend Vector3 operator*(const T &r, const Vector3 &v) { return Vector3(v.x * r, v.y * r, v.z * r); }
        friend std::ostream &operator<<(std::ostream &os, const Vector3 &v) { return os << v.x << ", " << v.y << ", " << v.z; }

        double dot(const Vector3 &v) const { return x * v.x + y * v.y + z * v.z; }
        Vector3 cross(const Vector3 &v) const { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
        double len() const { return std::sqrt(x * x + y * y + z * z); }
        Vector3 lerp(Vector3 &b, const double &t) const
        {
            double tt = std::min(std::max(0.0, t), 1.0);
            return (*this) * (1 - tt) + b * tt;
        }
        Vector3 normalized() const
        {
            if (x == 0 && y == 0 && z == 0)
            {
                return Vector3(0);
            }
            return *this / len();
        }

        T x, y, z;
    };

    template <typename T>
    class Vector2
    {
    public:
        Vector2() : x(0), y(0) {}
        Vector2(T one) : x(one), y(one) {}
        Vector2(T xx, T yy) : x(xx), y(yy) {}
        Vector2 operator*(const T &r) const { return Vector2(x * r, y * r); }
        Vector2 operator/(const T &r) const { return Vector2(x / r, y / r); }

        Vector2 operator*(const Vector2 &v) const { return Vector2(x * v.x, y * v.y); }
        Vector2 operator-(const Vector2 &v) const { return Vector2(x - v.x, y - v.y); }
        Vector2 operator+(const Vector2 &v) const { return Vector2(x + v.x, y + v.y); }
        Vector2 operator-() const { return Vector2(-x, -y); }
        Vector2 &operator+=(const Vector2 &v)
        {
            x += v.x, y += v.y, z += v.z;
            return *this;
        }
        friend Vector2 operator*(const T &r, const Vector2 &v) { return Vector2(v.x * r, v.y * r); }
        friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) { return os << v.x << ", " << v.y; }

        double dot(const Vector2 &v) const { return x * v.x + y * v.y; }
        double len() const { return std::sqrt(x * x + y * y); }
        Vector2 normalized() const
        {
            if (x == 0 && y == 0)
            {
                return Vector2(0);
            }
            return *this / len();
        }

        T x, y, z;
    };

    using Vector2D = Vector2<double>;
    using Vector2I = Vector2<int>;
    using Vector3D = Vector3<double>;
};