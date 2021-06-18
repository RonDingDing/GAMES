#pragma once
#include "VectorN.hpp"

namespace Rasterizer
{
    class Triangle3D
    {
    public:
        Vector3D a = Vector3D(0), b = Vector3D(0), c = Vector3D(0), normal = Vector3D(0), ab = Vector3D(0), bc = Vector3D(0), ca = Vector3D(0);
        Triangle3D(const Vector3D &a1, const Vector3D &b2, const Vector3D &c3) : a(a1), b(b2), c(c3), ab(b2 - a1), bc(c3 - b2), ca(a1 - c3)
        {
            normal = (ab ^ bc).normalized();
        }
    };

    class Triangle2I
    {
    public:
        Vector2I a = Vector2I(0), b = Vector2I(0), c = Vector2I(0), ab = Vector2I(0), bc = Vector2I(0), ca = Vector2I(0);
        Triangle2I(const Vector2I &a1, const Vector2I &b2, const Vector2I &c3) : a(a1), b(b2), c(c3), ab(b2 - a1), bc(c3 - b2), ca(a1 - c3)
        {
        }

        Vector3I barycentric(const Vector2I &p) const
        {
            Vector2I ap(p.x - a.x, p.y - a.y);
            Vector2I bp(p.x - b.x, p.y - b.y);
            Vector2I cp(p.x - c.x, p.y - c.y);
            return Vector3I(ab ^ ap, bc ^ bp, ca ^ cp);
        }

        Vector2I minimum() const
        {
            return Vector2I(std::min({a.x, b.x, c.x}), std::min({a.y, b.y, c.y}));
        }

        Vector2I maximum() const
        {
            return Vector2I(std::max({a.x, b.x, c.x}), std::max({a.y, b.y, c.y}));
        }
    };
}