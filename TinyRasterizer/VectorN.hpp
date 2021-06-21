#pragma once
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

namespace Rasterizer
{
    using Size = size_t;

    template <Size dimension_column, Size dimension_row, typename Number>
    class Mat;

    template <Size dimension, typename Number>
    class VectorN
    {
    public:
        VectorN()
        {
            for (Size i = 0; i < dimension; i++)
            {
                data[i] = Number();
            }
        }

        Number &operator[](const Size i)
        {
            assert(i < dimension);
            return data[i];
        }

        const Number &operator[](const Size i) const
        {
            assert(i < dimension);
            return data[i];
        }

    private:
        Number data[dimension];
    };

    /////////////////////////////////////////////////////////////////////////////////

    template <typename Number>
    class VectorN<2, Number>
    {
    public:
        VectorN() : x(Number()), y(Number()) {}

        VectorN(Number xx, Number yy) : x(xx), y(yy) {}

        VectorN(Number i) : x(i), y(i) {}

        template <typename U>
        VectorN<2, Number>(const VectorN<2, U> &v);

        Number &operator[](const Size i)
        {
            assert(i < 2);
            return i <= 0 ? x : y;
        }
        const Number &operator[](const Size i) const
        {
            assert(i < 2);
            return i <= 0 ? x : y;
        }

        Number x, y;
    };

    // /////////////////////////////////////////////////////////////////////////////////

    template <typename Number>
    class VectorN<3, Number>
    {
    public:
        VectorN() : x(Number()), y(Number()), z(Number()) {}

        VectorN(Number i) : x(Number(i)), y(Number(i)), z(Number(i)) {}

        VectorN(Number X, Number Y, Number Z) : x(X), y(Y), z(Z) {}

        template <class U>
        VectorN(const std::vector<U> &v) : x((Number)(v[0])), y((Number)(v[1])), z((Number)(v[2])) {}

        template <class U>
        VectorN<3, Number>(const VectorN<3, U> &v);

        Number &operator[](const Size i)
        {
            assert(i < 3);
            return i <= 0 ? x : (1 == i ? y : z);
        }

        const Number &operator[](const Size i) const
        {
            assert(i < 3);
            return i <= 0 ? x : (1 == i ? y : z);
        }

        Number norm() const { return std::sqrt(x * x + y * y + z * z); }

        VectorN<3, Number> &normalize(Number l = 1)
        {
            if (x == y && y == z && z == 0)
            {
                return *this;
            }
            *this = (*this) * (l / norm());
            return *this;
        }

        VectorN<3, Number> const normalized(Number l = 1) const
        {
            if (x == y && y == z && z == 0)
            {
                return VectorN<3, Number>(x, y, z);
            }
            return VectorN<3, Number>(x, y, z) * (l / norm());
        }

        VectorN<3, Number> operator^(const VectorN<3, Number> &v2)
        {
            return VectorN<3, Number>(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);
        }

        Number x, y, z;
    };

    //     // /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension, typename Number>
    Number operator*(const VectorN<dimension, Number> &lhs, const VectorN<dimension, Number> &rhs)
    {
        Number ret = Number();
        for (Size i = 0; i < dimension; i++)
        {
            ret += lhs[i] * rhs[i];
        }
        return ret;
    }

    template <Size dimension, typename Number>
    VectorN<dimension, Number> operator+(VectorN<dimension, Number> lhs, const VectorN<dimension, Number> &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] += rhs[i];
        }
        return lhs;
    }

    template <Size dimension, typename Number>
    VectorN<dimension, Number> operator-(VectorN<dimension, Number> lhs, const VectorN<dimension, Number> &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] -= rhs[i];
        }
        return lhs;
    }

    template <Size dimension, typename Number, typename U>
    VectorN<dimension, Number> operator*(VectorN<dimension, Number> lhs, const U &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] *= rhs;
        }
        return lhs;
    }

    template <Size dimension, typename Number, typename U>
    VectorN<dimension, Number> operator/(VectorN<dimension, Number> lhs, const U &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] /= rhs;
        }
        return lhs;
    }

    template <Size length, Size dimension, typename Number>
    VectorN<length, Number> embed(const VectorN<dimension, Number> &v, Number fill = 0)
    {
        VectorN<length, Number> ret;
        for (Size i = 0; i < length; i++)
        {
            ret[i] = (i < dimension ? v[i] : fill);
        }
        return ret;
    }

    template <Size length, Size dimension, typename Number>
    VectorN<length, Number> proj(const VectorN<dimension, Number> &v)
    {
        VectorN<length, Number> ret;
        for (Size i = 0; i < length; i++)
        {
            ret[i] = v[i];
        }
        return ret;
    }

    template <Size dimension, typename Number>
    std::ostream &operator<<(std::ostream &out, VectorN<dimension, Number> &v)
    {
        for (Size i = 0; i < dimension; i++)
        {
            out << v[i] << " ";
        }
        return out;
    }

    // /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension, typename Number>
    struct Dt
    {
        static Number det(const Mat<dimension, dimension, Number> &src)
        {
            Number ret = 0;
            for (Size i = 0; i < dimension; i++)
            {
                ret += src[0][i] * src.cofactor(0, i);
            }
            return ret;
        }
    };

    template <typename Number>
    struct Dt<1, Number>
    {
        static Number det(const Mat<1, 1, Number> &src)
        {
            return src[0][0];
        }
    };

    // /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension_row, Size dimension_column, typename Number>
    class Mat
    {
        VectorN<dimension_column, Number> rows[dimension_row];

    public:
        Mat() {}

        VectorN<dimension_column, Number> &operator[](const Size idx)
        {
            assert(idx < dimension_row);
            return rows[idx];
        }

        const VectorN<dimension_column, Number> &operator[](const Size idx) const
        {
            assert(idx < dimension_row);
            return rows[idx];
        }

        VectorN<dimension_row, Number> col(const Size idx) const
        {
            assert(idx < dimension_column);
            VectorN<dimension_row, Number> ret;
            for (Size i = 0; i < dimension_row; i++)
            {
                ret[i] = rows[i][idx];
            }
            return ret;
        }

        void set_col(Size idx, VectorN<dimension_row, Number> v)
        {
            assert(idx < dimension_column);
            for (Size i = 0; i < dimension_row; i++)
            {
                rows[i][idx] = v[i];
            }
        }

        static Mat<dimension_row, dimension_column, Number> identity()
        {
            Mat<dimension_row, dimension_column, Number> ret;
            for (Size i = 0; i < dimension_row; i++)
            {
                for (Size j = 0; j < dimension_column; j++)
                {
                    ret[i][j] = (i == j);
                }
            }
            return ret;
        }

        Number det() const
        {
            return Dt<dimension_column, Number>::det(*this);
        }

        Mat<dimension_row - 1, dimension_column - 1, Number> get_minor(Size row, Size col) const
        {
            Mat<dimension_row - 1, dimension_column - 1, Number> ret;
            for (Size i = 0; i < dimension_row - 1; i++)
            {
                for (Size j = 0; j < dimension_column - 1; j++)
                {
                    ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1];
                }
            }
            return ret;
        }

        Number cofactor(Size row, Size col) const
        {
            return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
        }

        Mat<dimension_row, dimension_column, Number> adjugate() const
        {
            Mat<dimension_row, dimension_column, Number> ret;
            for (Size i = 0; i < dimension_row - 1; i++)
            {
                for (Size j = 0; j < dimension_column - 1; j++)
                {
                    ret[i][j] = cofactor(i, j);
                }
            }
            return ret;
        }

        Mat<dimension_row, dimension_column, Number> invert_transpose()
        {
            Mat<dimension_row, dimension_column, Number> ret = adjugate();
            Number tmp = ret[0] * rows[0];
            return ret / tmp;
        }

        Mat<dimension_row, dimension_column, Number> invert()
        {
            return invert_transpose().transpose();
        }

        Mat<dimension_column, dimension_row, Number> transpose()
        {
            Mat<dimension_column, dimension_row, Number> ret;
            for (Size i = 0; i < dimension_row - 1; i++)
            {
                ret[i] = this->col(i);
            }
            return ret;
        }
    };

    //     /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension_row, Size dimension_column, typename Number>
    VectorN<dimension_row, Number> operator*(const Mat<dimension_row, dimension_column, Number> &lhs, const VectorN<dimension_column, Number> &rhs)
    {
        VectorN<dimension_row, Number> ret;
        for (Size i = 0; i < dimension_row; i++)
        {
            ret[i] = lhs[i] * rhs;
        }
        return ret;
    }

    template <Size R1, Size C1, Size C2, typename Number>
    Mat<R1, C2, Number> operator*(const Mat<R1, C1, Number> &lhs, const Mat<C1, C2, Number> &rhs)
    {
        Mat<R1, C2, Number> result;
        for (Size i = 0; i < R1; i++)
        {
            for (Size j = 0; j < C2; j++)
            {
                result[i][j] = lhs[i] * rhs.col(j);
            }
        }
        return result;
    }

    template <Size dimension_row, Size dimension_column, typename Number>
    Mat<dimension_column, dimension_row, Number> operator/(Mat<dimension_row, dimension_column, Number> lhs, const Number &rhs)
    {
        for (Size i = 0; i < dimension_row; i++)
        {
            lhs[i] = lhs[i] / rhs;
        }
        return lhs;
    }

    template <Size dimension_row, Size dimension_column, class Number>
    std::ostream &operator<<(std::ostream &out, Mat<dimension_row, dimension_column, Number> &m)
    {
        for (Size i = 0; i < dimension_row; i++)
        {
            out << m[i] << std::endl;
        }
        return out;
    }

    // /////////////////////////////////////////////////////////////////////////////////

    typedef VectorN<2, double> Vector2D;
    typedef VectorN<2, int> Vector2I;
    typedef VectorN<3, double> Vector3D;
    typedef VectorN<3, int> Vector3I;
    typedef VectorN<4, double> Vector4D;
    typedef VectorN<4, int> Vector4I;
    typedef Mat<4, 4, double> Matrix4D;

    template <typename Number>
    class Color
    {
    public:
        Vector3D rgb;
        Number a;

        Color(Number r, Number g, Number b, Number al) : rgb({r, g, b}), a(al) {}
    };
}