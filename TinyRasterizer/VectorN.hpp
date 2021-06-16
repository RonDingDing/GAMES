#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>

class Vector3D
{
public:
    double x, y, z;
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double xx) : x(xx), y(xx), z(xx) {}
    Vector3D(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}
    Vector3D operator*(const double &r) const { return Vector3D(x * r, y * r, z * r); }
    Vector3D operator/(const double &r) const { return Vector3D(x / r, y / r, z / r); }
    double len() { return std::sqrt(x * x + y * y + z * z); }
    double len_square() { return x * x + y * y + z * z; }
    Vector3D normalized()
    {
        if (x == y && y == z && z == 0)
            return Vector3D(0);
        double n = std::sqrt(x * x + y * y + z * z);
        return Vector3D(x / n, y / n, z / n);
    }
    Vector3D operator*(const Vector3D &v) const { return Vector3D(x * v.x, y * v.y, z * v.z); }
    Vector3D operator-(const Vector3D &v) const { return Vector3D(x - v.x, y - v.y, z - v.z); }
    Vector3D operator+(const Vector3D &v) const { return Vector3D(x + v.x, y + v.y, z + v.z); }
    Vector3D operator-() const { return Vector3D(-x, -y, -z); }
    Vector3D &operator+=(const Vector3D &v)
    {
        x += v.x, y += v.y, z += v.z;
        return *this;
    }
    friend Vector3D operator*(const double &r, const Vector3D &v) { return Vector3D(v.x * r, v.y * r, v.z * r); }
    friend std::ostream &operator<<(std::ostream &os, const Vector3D &v) { return os << "(" << v.x << ", " << v.y << ", " << v.z << ")"; }
    static Vector3D mins(const Vector3D &p1, const Vector3D &p2) { return Vector3D(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)); }
    static Vector3D maxs(const Vector3D &p1, const Vector3D &p2) { return Vector3D(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)); }
    static Vector3D lerp(const Vector3D &a, const Vector3D &b, const double &t) { return a * (1 - t) + b * t; }
    double dot(const Vector3D &b) { return x * b.x + y * b.y + z * b.z; }
    Vector3D cross(const Vector3D &b) { return Vector3D(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }
};

class Vector2D
{
public:
    Vector2D() : x(0), y(0) {}
    Vector2D(double xx) : x(xx), y(xx) {}
    Vector2D(double xx, double yy) : x(xx), y(yy) {}
    Vector2D operator*(const double &r) const { return Vector2D(x * r, y * r); }
    Vector2D operator+(const Vector2D &v) const { return Vector2D(x + v.x, y + v.y); }
    Vector2D operator-() const { return Vector2D(-x, -y); }
    Vector2D operator-(const Vector2D &v) const { return Vector2D(x - v.x, y - v.y); }

    double dot(const Vector2D &b) { return x * b.x + y * b.y; }
    double x, y;
};
