#pragma once
#include "VectorN.hpp"

namespace Rasterizer
{
    class Triangle
    {
    public:
        Vector3D p1 = Vector3D(0), p2 = Vector3D(0), p3 = Vector3D(0), normal = Vector3D(0), p1p2 = Vector3D(0), p2p3 = Vector3D(0), p3p1 = Vector3D(0);
        Triangle(const Vector3D &p11, const Vector3D &p22, const Vector3D &p33) : p1(p11), p2(p22), p3(p33), p1p2(p22 - p11), p2p3(p33 - p22), p3p1(p11 - p33)
        {
            normal = p1p2.cross(p2p3).normalized();
        }
    };
}