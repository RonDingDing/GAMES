#pragma once
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

namespace Rasterizer
{
    using Size = size_t;

    template <Size dimension_column, Size dimension_row, typename Scalar>
    class Mat;

    template <Size dimension, typename Scalar>
    class VectorN
    {
    public:
        VectorN()
        {
            for (Size i = 0; i < dimension; i++)
            {
                data[i] = Scalar();
            }
        }

        Scalar &operator[](const Size i)
        {
            assert(i < dimension);
            return data[i];
        }

        const Scalar &operator[](const Size i) const
        {
            assert(i < dimension);
            return data[i];
        }

    private:
        Scalar data[dimension];
    };

    /////////////////////////////////////////////////////////////////////////////////

    template <typename Scalar>
    class VectorN<2, Scalar>
    {
    public:
        VectorN() : x(Scalar()), y(Scalar()) {}

        VectorN(Scalar xx, Scalar yy) : x(xx), y(yy) {}

        VectorN(Scalar i) : x(i), y(i) {}

        template <typename U>
        VectorN<2, Scalar>(const VectorN<2, U> &v);

        Scalar &operator[](const Size i)
        {
            assert(i < 2);
            return i <= 0 ? x : y;
        }
        const Scalar &operator[](const Size i) const
        {
            assert(i < 2);
            return i <= 0 ? x : y;
        }

        Scalar x, y;
    };

    // /////////////////////////////////////////////////////////////////////////////////

    template <typename Scalar>
    class VectorN<3, Scalar>
    {
    public:
        VectorN() : x(Scalar()), y(Scalar()), z(Scalar()) {}

        VectorN(Scalar i) : x(Scalar(i)), y(Scalar(i)), z(Scalar(i)) {}

        VectorN(Scalar X, Scalar Y, Scalar Z) : x(X), y(Y), z(Z) {}

        template <Size matrix_dimension_row, Size matrix_dimension_column>
        VectorN<3, Scalar>(const Mat<matrix_dimension_row, matrix_dimension_column, Scalar> &m)
            : x(m[3][0] != 0 ? m[0][0] / m[3][0] : m[0][0]), y(m[3][0] != 0 ? m[1][0] / m[3][0] : m[1][0]), z(m[3][0] != 0 ? m[2][0] / m[3][0] : m[2][0]) {}

        template <class U>
        VectorN(const std::vector<U> &v) : x((Scalar)(v[0])), y((Scalar)(v[1])), z((Scalar)(v[2])) {}

        template <class U>
        VectorN<3, Scalar>(const VectorN<3, U> &v);

        Scalar &operator[](const Size i)
        {
            assert(i < 3);
            return i <= 0 ? x : (1 == i ? y : z);
        }

        const Scalar &operator[](const Size i) const
        {
            assert(i < 3);
            return i <= 0 ? x : (1 == i ? y : z);
        }

        Scalar norm() const { return std::sqrt(x * x + y * y + z * z); }

        VectorN<3, Scalar> &normalize(Scalar l = 1)
        {
            if (x == y && y == z && z == 0)
            {
                return *this;
            }
            *this = (*this) * (l / norm());
            return *this;
        }

        VectorN<3, Scalar> const normalized(Scalar l = 1) const
        {
            if (x == y && y == z && z == 0)
            {
                return VectorN<3, Scalar>(x, y, z);
            }
            return VectorN<3, Scalar>(x, y, z) * (l / norm());
        }

        VectorN<3, Scalar> operator^(const VectorN<3, Scalar> &v2) const
        {
            return VectorN<3, Scalar>(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);
        }

        Scalar x, y, z;
    };

    //     // /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension, typename Scalar>
    Scalar operator*(const VectorN<dimension, Scalar> &lhs, const VectorN<dimension, Scalar> &rhs)
    {
        Scalar ret = Scalar();
        for (Size i = 0; i < dimension; i++)
        {
            ret += lhs[i] * rhs[i];
        }
        return ret;
    }

    template <Size dimension, typename Scalar>
    VectorN<dimension, Scalar> operator+(VectorN<dimension, Scalar> lhs, const VectorN<dimension, Scalar> &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] += rhs[i];
        }
        return lhs;
    }

    template <Size dimension, typename Scalar>
    VectorN<dimension, Scalar> operator-(VectorN<dimension, Scalar> lhs, const VectorN<dimension, Scalar> &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] -= rhs[i];
        }
        return lhs;
    }

    template <Size dimension, typename Scalar, typename U>
    VectorN<dimension, Scalar> operator*(VectorN<dimension, Scalar> lhs, const U &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] *= rhs;
        }
        return lhs;
    }

    template <Size dimension, typename Scalar, typename U>
    VectorN<dimension, Scalar> operator/(VectorN<dimension, Scalar> lhs, const U &rhs)
    {
        for (Size i = 0; i < dimension; i++)
        {
            lhs[i] /= rhs;
        }
        return lhs;
    }

    template <Size length, Size dimension, typename Scalar>
    VectorN<length, Scalar> embed(const VectorN<dimension, Scalar> &v, Scalar fill = 0)
    {
        VectorN<length, Scalar> ret;
        for (Size i = 0; i < length; i++)
        {
            ret[i] = (i < dimension ? v[i] : fill);
        }
        return ret;
    }

    template <Size length, Size dimension, typename Scalar>
    VectorN<length, Scalar> proj(const VectorN<dimension, Scalar> &v)
    {
        VectorN<length, Scalar> ret;
        for (Size i = 0; i < length; i++)
        {
            ret[i] = v[i];
        }
        return ret;
    }

    template <Size dimension, typename Scalar>
    std::ostream &operator<<(std::ostream &out, VectorN<dimension, Scalar> &v)
    {
        for (Size i = 0; i < dimension; i++)
        {
            out << v[i] << " ";
        }
        return out;
    }

    // /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension, typename Scalar>
    struct Dt
    {
        static Scalar det(const Mat<dimension, dimension, Scalar> &src)
        {
            Scalar ret = 0;
            for (Size i = 0; i < dimension; i++)
            {
                ret += src[0][i] * src.cofactor(0, i);
            }
            return ret;
        }
    };

    template <typename Scalar>
    struct Dt<1, Scalar>
    {
        static Scalar det(const Mat<1, 1, Scalar> &src)
        {
            return src[0][0];
        }
    };

    // /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension_row, Size dimension_column, typename Scalar>
    class Mat
    {
        VectorN<dimension_column, Scalar> rows[dimension_row];
        Size pointer_row = 0;
        Size pointer_col = 0;

    public:
        Mat() {}

        template <Size vector_dimension>
        Mat(const VectorN<vector_dimension, Scalar> &vec, const bool &zero = false)
        {

            for (Size i = 0; i < dimension_row; i++)
            {
                for (Size j = 0; j < dimension_column; j++)
                {
                    rows[i][j] = (Scalar)(zero ? 0 : 1);
                }
            }
            assert(vector_dimension <= dimension_row);
            for (Size i = 0; i < vector_dimension; i++)
            {
                rows[i][0] = vec[i];
            }
        }

        VectorN<dimension_column, Scalar> &operator[](const Size idx)
        {
            assert(idx < dimension_row);
            return rows[idx];
        }

        const VectorN<dimension_column, Scalar> &operator[](const Size idx) const
        {
            assert(idx < dimension_row);
            return rows[idx];
        }

        VectorN<dimension_row, Scalar> col(const Size idx) const
        {
            assert(idx < dimension_column);
            VectorN<dimension_row, Scalar> ret;
            for (Size i = 0; i < dimension_row; i++)
            {
                ret[i] = rows[i][idx];
            }
            return ret;
        }

        void set_col(Size idx, VectorN<dimension_row, Scalar> v)
        {
            assert(idx < dimension_column);
            for (Size i = 0; i < dimension_row; i++)
            {
                rows[i][idx] = v[i];
            }
        }

        static Mat<dimension_row, dimension_column, Scalar> identity()
        {
            Mat<dimension_row, dimension_column, Scalar> ret;
            for (Size i = 0; i < dimension_row; i++)
            {
                for (Size j = 0; j < dimension_column; j++)
                {
                    ret[i][j] = (i == j);
                }
            }
            return ret;
        }

        Scalar det() const
        {
            return Dt<dimension_column, Scalar>::det(*this);
        }

        Mat<dimension_row - 1, dimension_column - 1, Scalar> get_minor(Size row, Size col) const
        {
            Mat<dimension_row - 1, dimension_column - 1, Scalar> ret;
            for (Size i = 0; i < dimension_row - 1; i++)
            {
                for (Size j = 0; j < dimension_column - 1; j++)
                {
                    ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1];
                }
            }
            return ret;
        }

        Scalar cofactor(Size row, Size col) const
        {
            return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
        }

        Mat<dimension_row, dimension_column, Scalar> adjugate() const
        {
            Mat<dimension_row, dimension_column, Scalar> ret;
            for (Size i = 0; i < dimension_row - 1; i++)
            {
                for (Size j = 0; j < dimension_column - 1; j++)
                {
                    ret[i][j] = cofactor(i, j);
                }
            }
            return ret;
        }

        Mat<dimension_row, dimension_column, Scalar> invert_transpose()
        {
            Mat<dimension_row, dimension_column, Scalar> ret = adjugate();
            Scalar tmp = ret[0] * rows[0];
            return ret / tmp;
        }

        Mat<dimension_row, dimension_column, Scalar> invert()
        {
            return invert_transpose().transpose();
        }

        Mat<dimension_column, dimension_row, Scalar> transpose()
        {
            Mat<dimension_column, dimension_row, Scalar> ret;
            for (Size i = 0; i < dimension_row - 1; i++)
            {
                ret[i] = this->col(i);
            }
            return ret;
        }

        void operator||(const Scalar &p)
        {
            pointer_col = p;
            pointer_row = p;
        }

        Mat<dimension_column, dimension_row, Scalar> &operator<<(const Scalar &k)
        {
            (*this)[pointer_row][pointer_col] = k;
            pointer_col++;
            if (pointer_col == dimension_row)
            {
                pointer_row++;
                pointer_col = 0;
                if (pointer_row == dimension_column)
                {
                    pointer_row = 0;
                }
            }
            return *this;
        }
    };

    //     /////////////////////////////////////////////////////////////////////////////////

    template <Size dimension_row, Size dimension_column, typename Scalar>
    VectorN<dimension_row, Scalar> operator*(const Mat<dimension_row, dimension_column, Scalar> &lhs, const VectorN<dimension_column, Scalar> &rhs)
    {
        VectorN<dimension_row, Scalar> ret;
        for (Size i = 0; i < dimension_row; i++)
        {
            ret[i] = lhs[i] * rhs;
        }
        return ret;
    }

    template <Size R1, Size C1, Size C2, typename Scalar>
    Mat<R1, C2, Scalar> operator*(const Mat<R1, C1, Scalar> &lhs, const Mat<C1, C2, Scalar> &rhs)
    {
        Mat<R1, C2, Scalar> result;
        for (Size i = 0; i < R1; i++)
        {
            for (Size j = 0; j < C2; j++)
            {
                result[i][j] = lhs[i] * rhs.col(j);
            }
        }
        return result;
    }

    template <Size dimension_row, Size dimension_column, typename Scalar>
    Mat<dimension_column, dimension_row, Scalar> operator/(Mat<dimension_row, dimension_column, Scalar> lhs, const Scalar &rhs)
    {
        for (Size i = 0; i < dimension_row; i++)
        {
            lhs[i] = lhs[i] / rhs;
        }
        return lhs;
    }

    template <Size dimension_row, Size dimension_column, class Scalar>
    std::ostream &operator<<(std::ostream &out, Mat<dimension_row, dimension_column, Scalar> &m)
    {
        for (Size i = 0; i < dimension_row; i++)
        {
            out << m[i] << "\t" << std::endl;
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
    typedef Mat<4, 4, double> Matrix4D4;
    typedef Mat<4, 1, double> Matrix4D1;

    template <typename Scalar>
    class Color
    {
    public:
        Vector3D rgb;
        Scalar a;

        Color(Scalar r, Scalar g, Scalar b, Scalar al) : rgb({r, g, b}), a(al) {}
        Color(Scalar r, Scalar g, Scalar b) : rgb({r, g, b}), a(1.0) {}
    };
}