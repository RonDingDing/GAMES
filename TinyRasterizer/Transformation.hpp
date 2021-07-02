#include <VectorN.hpp>

namespace Rasterizer
{
    class Transformation
    {
    private:
        static Matrix4D &viewport(const int &x, const int &y, const int &w, const int &h, const int &depth)
        {
            Matrix4D m;
            m << w / 2.0 << 0.0 << 0.0 << x + w / 2.0
              << 0.0 << h / 2.0 << 0.0 << y + h / 2.0
              << 0.0 << 0.0 << depth / 2.0 << depth / 2.0
              << 0.0 << 0.0 << 0.0 << 1.0;
            m.reset_pointer();
            return m;
        }

        static Matrix4D &lookat(const Vector3D &eye, const Vector3D &center, const Vector3D &up)
        {
            Vector3D z = (eye - center).normalize();
            Vector3D x = (up ^ z).normalize();
            Vector3D y = (z ^ x).normalize();

            Matrix4D m;
            m << x.x << y.x << z.x << 0.0
              << x.y << y.y << z.y << 0.0
              << x.z << y.z << z.z << 0.0
              << -center.x << -center.y << -center.z << 1.0;
            m.reset_pointer();
            return m;
        }

        static Matrix4D &project(const Vector3D &eye, const Vector3D &center)
        {
            Matrix4D m;
            m << 1.0 << 0.0 << 0.0 << 0.0
              << 0.0 << 1.0 << 0.0 << 0.0
              << 0.0 << 0.0 << 1.0 << -1.f / (eye - center).norm()
              << 0.0 << 0.0 << 0.0 << 1.0;
            m.reset_pointer();
            return m;
        }

    public:
        static Matrix4D get_matrix(const int &x, const int &y, const int &w, const int &h, const int &depth, const Vector3D &eye, const Vector3D &center, const Vector3D &up)
        {
            Matrix4D modelview = lookat(eye, center, up);
            Matrix4D projection = project(eye, center);
            Matrix4D viewports = viewport(w / 8, h / 8, w * 3 / 4, h * 3 / 4, depth);
            return viewports * projection * modelview;
        }
    };
}