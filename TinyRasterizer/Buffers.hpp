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
            // pixel[width * yy + xx] = color;
            pixel[(height - yy) * width + xx] = color;
        }

        const Vector3D &get_pixel_color(const int &xx, const int &yy)
        {
            if (xx > width - 1 || xx < 0 || yy > height - 1 || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }

            // return pixel[width * yy + xx];
            return pixel[(height - yy) * width + xx];
        }

        void set_depth_value(const int &xx, const int &yy, const double &dep)
        {
            if (xx > width - 1 || xx < 0 || yy > height - 1 || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return depth[width * yy + xx];
            depth[(height - yy) * width + xx] = dep;
        }

        const double &get_depth_value(const int &xx, const int &yy)
        {
            if (xx > width - 1 || xx < 0 || yy > height - 1 || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return depth[width * yy + xx];
            return depth[(height - yy) * width + xx];
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

        void draw_line(const Vector2D &p1, const Vector2D &p2, const Vector3D &color)
        {
            int x0 = p1.x, x1 = p2.x, y0 = p1.y, y1 = p2.y;
            bool steep = false;
            if (std::abs(x0 - x1) < std::abs(y0 - y1))
            {
                std::swap(x0, y0);
                std::swap(x1, y1);
                steep = true;
            }

            if (x0 > x1)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }

            int dx = x1 - x0;
            int dy = y1 - y0;
            int derror2 = std::abs(dy) * 2;
            int error2 = 0;
            int y = y0;
            for (int x = x0; x <= x1; x++)
            {
                if (steep)
                {
                    set_pixel_color(y, x, color);
                }
                else
                {
                    set_pixel_color(x, y, color);
                }
                error2 += derror2;
                if (error2 > dx)
                {
                    y += (y1 > y0 ? 1 : -1);
                    error2 -= dx * 2;
                }
            }
        }
    };
}