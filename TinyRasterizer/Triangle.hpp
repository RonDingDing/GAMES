#pragma once
#include "VectorN.hpp"

namespace Rasterizer
{
    class Triangle
    {
    public:
        Vector3D a = Vector3D(0), b = Vector3D(0), c = Vector3D(0), normal = Vector3D(0), ab = Vector3D(0), bc = Vector3D(0), ca = Vector3D(0);
        Triangle(const Vector3D &a1, const Vector3D &b2, const Vector3D &c3) : a(a1), b(b2), c(c3), ab(b2 - a1), bc(c3 - b2), ca(a1 - c3)
        {
            normal = ab.cross(bc).normalized();
        }
    };
}