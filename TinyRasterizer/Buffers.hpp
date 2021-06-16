#pragma once
#include <vector>
#include <stdexcept>
#include "VectorN.hpp"

namespace Rasterizer
{
    class Buffers
    {
    public:
        std::vector<double> depth;
        std::vector<Vector3D> pixel;
        int width = 0;
        int height = 0;

        Buffers(const int &xx, const int &yy)
        {
            resize(xx, yy);
        }

        void set_pixel_color(const int &xx, const int &yy, const Vector3D &color)
        {
            if (xx > width - 1 || xx < 0 || yy > height - 1 || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            pixel[width * yy + xx] = color;
        }

        const Vector3D &get_pixel_color(const int &xx, const int &yy)
        {
            if (xx > width - 1 || xx < 0 || yy > height - 1 || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            return pixel[width * yy + xx];
        }

        const double &get_depth_value(const int &xx, const int &yy)
        {
            if (xx > width - 1 || xx < 0 || yy > height - 1 || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            return depth[width * yy + xx];
        }

        void resize(const int &xx, const int &yy)
        {
            width = std::max(std::max(xx, 1), std::max(yy, 1));
            height = std::min(std::max(xx, 1), std::max(yy, 1));
            depth.resize(width * height);
            clear_depth();

            pixel.resize(width * height);
            clear_pixel();
        }

        void clear_pixel()
        {
            std::fill(pixel.begin(), pixel.end(), Vector3D(0));
        }

        void clear_depth()
        {
            std::fill(depth.begin(), depth.end(), 0);
        }

        void print_pixel()
        {
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    std::cout << get_pixel_color(i, j) << " ";
                }
                std::cout << std::endl;
            }
        }

        void print_depth()
        {
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    std::cout << get_depth_value(i, j) << " ";
                }
                std::cout << std::endl;
            }
        }
    };
}