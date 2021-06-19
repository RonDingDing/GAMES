#pragma once
#include "VectorN.hpp"

namespace Rasterizer
{
    using Number = double;
    using Num = int;
    class Triangle3D
    {
    public:
        Vector3D a = Vector3D(0), b = Vector3D(0), c = Vector3D(0), normal = Vector3D(0), ab = Vector3D(0), bc = Vector3D(0), ca = Vector3D(0);
        Triangle3D(const Vector3D &a1, const Vector3D &b2, const Vector3D &c3) : a(a1), b(b2), c(c3), ab(b2 - a1), bc(c3 - b2), ca(a1 - c3)
        {
            normal = (ab ^ bc).normalized();
        }

        Vector3D barycentric(const Vector3D &p) const
        {
            double yb_yc = b.y - c.y;
            double xc_xb = c.x - b.x;
            double yc_ya = c.y - a.y;
            double xa_xc = a.x - c.x;
            double ya_yb = a.y - b.y;
            double xb_xa = b.x - a.x;

            double c1 = (p.x * yb_yc + xc_xb * p.y + b.x * c.y - c.x * b.y) / (a.x * yb_yc + xc_xb * a.y + b.x * c.y - c.x * b.y);
            double c2 = (p.x * yc_ya + xa_xc * p.y + c.x * a.y - a.x * c.y) / (b.x * yc_ya + xa_xc * b.y + c.x * a.y - a.x * c.y);
            double c3 = (p.x * ya_yb + xb_xa * p.y + a.x * b.y - b.x * a.y) / (c.x * ya_yb + xb_xa * c.y + a.x * b.y - b.x * a.y);

            return Vector3D(c1, c2, c3);
        }

        std::vector<Number> minimum() const
        {
            Number x_min = std::numeric_limits<Number>::max();
            Number y_min = std::numeric_limits<Number>::max();

            x_min = (Number)a.x;
            if (x_min > b.x)
                x_min = (Number)b.x;
            if (x_min > c.x)
                x_min = (Number)c.x;

            y_min = (Number)a.y;
            if (y_min > b.y)
                y_min = (Number)b.y;
            if (y_min > c.y)
                y_min = (Number)c.y;
            return {x_min, y_min};
        }

        std::vector<Number> maximum() const
        {
            Number x_max = std::numeric_limits<Number>::min();
            Number y_max = std::numeric_limits<Number>::min();

            x_max = (Number)a.x;
            if (x_max < b.x)
                x_max = (Number)b.x;
            if (x_max < c.x)
                x_max = (Number)c.x;

            y_max = (Number)a.y;
            if (y_max < b.y)
                y_max = (Number)b.y;
            if (y_max < c.y)
                y_max = (Number)c.y;
            return {x_max, y_max};
        }
    };

    class Triangle2I
    {
    public:
        Vector2I a = Vector2I(0), b = Vector2I(0), c = Vector2I(0), ab = Vector2I(0), bc = Vector2I(0), ca = Vector2I(0);
        Triangle2I(const Vector2I &a1, const Vector2I &b2, const Vector2I &c3) : a(a1), b(b2), c(c3), ab(b2 - a1), bc(c3 - b2), ca(a1 - c3)
        {
        }

        Vector3D barycentric(const Vector2I &p) const
        {

            double yb_yc = b.y - c.y;
            double xc_xb = c.x - b.x;
            double yc_ya = c.y - a.y;
            double xa_xc = a.x - c.x;
            double ya_yb = a.y - b.y;
            double xb_xa = b.x - a.x;

            double c1 = (p.x * yb_yc + xc_xb * p.y + b.x * c.y - c.x * b.y) / (a.x * yb_yc + xc_xb * a.y + b.x * c.y - c.x * b.y);
            double c2 = (p.x * yc_ya + xa_xc * p.y + c.x * a.y - a.x * c.y) / (b.x * yc_ya + xa_xc * b.y + c.x * a.y - a.x * c.y);
            double c3 = (p.x * ya_yb + xb_xa * p.y + a.x * b.y - b.x * a.y) / (c.x * ya_yb + xb_xa * c.y + a.x * b.y - b.x * a.y);

            return Vector3D(c1, c2, c3);
        }

        std::vector<Num> minimum() const
        {
            Num x_min = std::numeric_limits<Num>::max();
            Num y_min = std::numeric_limits<Num>::max();

            x_min = (Num)a.x;
            if (x_min > b.x)
                x_min = (Num)b.x;
            if (x_min > c.x)
                x_min = (Num)c.x;

            y_min = (Num)a.y;
            if (y_min > b.y)
                y_min = (Num)b.y;
            if (y_min > c.y)
                y_min = (Num)c.y;
            return {x_min, y_min};
        }

        std::vector<Num> maximum() const
        {
            Num x_max = std::numeric_limits<Num>::min();
            Num y_max = std::numeric_limits<Num>::min();

            x_max = (Num)a.x;
            if (x_max < b.x)
                x_max = (Num)b.x;
            if (x_max < c.x)
                x_max = (Num)c.x;

            y_max = (Num)a.y;
            if (y_max < b.y)
                y_max = (Num)b.y;
            if (y_max < c.y)
                y_max = (Num)c.y;
            return {x_max, y_max};
        }
    };
}