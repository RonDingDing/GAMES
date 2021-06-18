#pragma once
#include <vector>
#include <random>
#include <stdexcept>
#include "VectorN.hpp"
#include "Triangle.hpp"
#include "Mesh.hpp"

namespace Rasterizer
{
    using Num = double;
    class Buffers
    {
    public:
        std::vector<Num> depth;
        std::vector<Vector3D> pixel;
        int width = 0;
        int height = 0;

        Buffers(const int &xx, const int &yy)
        {
            resize(xx, yy);
        }

        void set_pixel_color(const int &xx, const int &yy, const Vector3D &color)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // pixel[width * yy + xx] = color;
            pixel[(height - yy) * width + xx] = color;
        }

        Vector3D &get_pixel_color(const int &xx, const int &yy)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return pixel[width * yy + xx];
            return pixel[(height - yy) * width + xx];
        }

        void set_depth_value(const int &xx, const int &yy, const Num &dep)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return depth[width * yy + xx];
            depth[(height - yy) * width + xx] = dep;
        }

        Num &get_depth_value(const int &xx, const int &yy)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
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

        void draw_line(const Vector2I &p1, const Vector2I &p2, const Vector3D &color)
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

        void draw_triangle_frame(const Triangle2I &tri, const Vector3D &color)
        {
            // 只画线框三角形
            draw_triangle_frame({tri.a.x, tri.a.y}, {tri.b.x, tri.b.y}, {tri.c.x, tri.c.y}, color);
        }

        void draw_triangle_frame(const Vector2I &a, const Vector2I &b, const Vector2I &c, const Vector3D &color)
        {
            // 只画线框三角形
            draw_line(a, b, color);
            draw_line(b, c, color);
            draw_line(c, a, color);
        }

        void draw_triangle_filled(const Vector2I &a, const Vector2I &b, const Vector2I &c, const Vector3D &color)
        {
            return draw_triangle_filled(Triangle2I(a, b, c), color);
        }

        void draw_triangle_filled(const Triangle2I &tri, const Vector3D &color)
        {
            // 画一个涂满颜色的三角形
            // 包围盒
            Vector2I max_vec = tri.maximum();
            Vector2I min_vec = tri.minimum();
            int x_max = std::min({max_vec.x, height - 1});
            int y_max = std::min({max_vec.y, height - 1});
            int x_min = std::max({min_vec.x, 0});
            int y_min = std::max({min_vec.y, 0});

            // 遍历包围盒
            Vector2I p;
            for (p.x = x_min; p.x <= x_max; p.x++)
            {
                for (p.y = y_min; p.y <= y_max; p.y++)
                {
                    Vector3I bc_screen = tri.barycentric(p);
                    // 重心在三角形外不渲染
                    if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                    {
                        continue;
                    }
                    set_pixel_color(p.x, p.y, color);
                }
            }
        }

        void set_mesh_frame(Mesh &mesh, const Vector3D &color)
        {
            // 只用指定颜色画线框
            for (int i = 0; i < mesh.face_num(); i++)
            {
                std::vector<int> face = mesh.faces[i];
                for (int j = 0; j < 3; j++)
                {
                    Vector3D v0 = mesh.vertices[face[j]];
                    Vector3D v1 = mesh.vertices[face[(j + 1) % 3]];
                    int x0 = (v0.x + 1.) * width / 2.;
                    int y0 = (v0.y + 1.) * height / 2.;
                    int x1 = (v1.x + 1.) * width / 2.;
                    int y1 = (v1.y + 1.) * height / 2.;
                    draw_line(Vector2I(x0, y0), Vector2I(x1, y1), color);
                }
            }
        }
        void set_mesh_filled(Mesh &mesh)
        {
            // 将模型的每个面都涂上随机颜色
            for (int i = 0; i < mesh.face_num(); i++)
            {
                std::vector<int> face = mesh.faces[i];
                Vector2I screen_coords[3];
                for (int j = 0; j < 3; j++)
                {
                    Vector3D world_coords = mesh.vertices[face[j]];
                    screen_coords[j] = Vector2I((world_coords.x + 1.) * width / 2., (world_coords.y + 1.) * height / 2.);
                }
                Triangle2I tri(screen_coords[0], screen_coords[1], screen_coords[2]);
                Vector3D color(rand() % 255, rand() % 255, rand() % 255);
                draw_triangle_filled(tri, color);
            }
        }

        void set_mesh_gray(Mesh &mesh, const Vector3D &light)
        {
            // 假装有一束光照亮他
            Vector3D light_dir = light.normalized();
            for (int i = 0; i < mesh.face_num(); i++)
            {
                std::vector<int> face = mesh.faces[i];
                Vector2I screen_coords[3];
                Vector3D world_coords[3];
                for (int j = 0; j < 3; j++)
                {
                    Vector3D v = mesh.vertices[face[j]];
                    screen_coords[j] = Vector2I((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
                    world_coords[j] = v;
                }
                Vector3D normal = Triangle3D(world_coords[0], world_coords[1], world_coords[2]).normal.normalized();
                Triangle2I tri(screen_coords[0], screen_coords[1], screen_coords[2]);
                double intensity = normal * light_dir;
                if (intensity > 0)
                {
                    // 默认光照是白色
                    Vector3D color(intensity * 255, intensity * 255, intensity * 255);
                    draw_triangle_filled(tri, color);
                }
            }
        }
    };
}