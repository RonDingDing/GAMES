#pragma once
#include "VectorN.hpp"

namespace Rasterizer
{
    class Transformation
    {
    public:
        static Matrix4D4 get_matrix(const int &x, const int &y, const int &w, const int &h, const int &depth, const Vector3D &eye, const Vector3D &center, const Vector3D &up)
        {
            Matrix4D4 modelview = lookat(eye, center, up);
            Matrix4D4 projection = project(eye, center);
            Matrix4D4 viewports = viewport(w / 8, h / 8, w * 3 / 4, h * 3 / 4, depth);
            return viewports * projection * modelview;
        }

        static Matrix4D4 viewport(const int &x, const int &y, const int &w, const int &h, const int &depth)
        {
            Matrix4D4 m;
            m << w / 2.0 << 0.0 << 0.0 << x + w / 2.0
              << 0.0 << h / 2.0 << 0.0 << y + h / 2.0
              << 0.0 << 0.0 << depth / 2.0 << depth / 2.0
              << 0.0 << 0.0 << 0.0 << 1.0 ||
                0;
            return m;
        }

        static Matrix4D4 lookat(const Vector3D &eye, const Vector3D &center, const Vector3D &up)
        {
            Vector3D zz = (eye - center).normalize();
            Vector3D xx = (up ^ zz).normalize();
            Vector3D yy = (zz ^ xx).normalize();

            Matrix4D4 min_v;
            min_v << xx.x << xx.y << xx.z << -center.x
                  << yy.x << yy.y << yy.z << -center.y
                  << zz.x << zz.y << zz.z << -center.z
                  << 0.0 << 0.0 << 0.0 << 1.0 ||
                0;

            return min_v;
        }

        static Matrix4D4 project(const Vector3D &eye, const Vector3D &center)
        {
            Matrix4D4 m;
            m << 1.0 << 0.0 << 0.0 << 0.0
              << 0.0 << 1.0 << 0.0 << 0.0
              << 0.0 << 0.0 << 1.0 << 0.0
              << 0.0 << 0.0 << -1.f / (eye - center).norm() << 1.0 ||
                0;
            return m;
        }
    };
}