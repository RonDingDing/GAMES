#pragma once
#include <vector>
#include <random>
#include <stdexcept>
#include <limits>
#include "VectorN.hpp"
#include "Triangle.hpp"
#include "Mesh.hpp"
#include <stdio.h>

namespace Rasterizer
{
    using Number = double;
    class Buffers
    {
    public:
        std::vector<Number> depth;
        std::vector<Vector3D> pixel;
        std::vector<Vector3D> texture;
        int width = 0;
        int height = 0;

        Buffers(const int &xx, const int &yy)
        {
            resize(xx, yy);
        }

        void set_pixel_color(const int &xx, const int &yy, Vector3D color)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // pixel[width * yy + xx] = color;
            pixel[(height - yy - 1) * width + xx] = color;
        }

        Vector3D &get_pixel_color(const int &xx, const int &yy)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return pixel[width * yy + xx];
            return pixel[(height - yy - 1) * width + xx];
        }

        void set_depth_value(const int &xx, const int &yy, const Number &dep)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return depth[width * yy + xx];
            depth[(height - yy - 1) * width + xx] = dep;
        }

        Number &get_depth_value(const int &xx, const int &yy)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return depth[width * yy + xx];
            return depth[(height - yy - 1) * width + xx];
        }

        void set_texture(const int &xx, const int &yy, const Vector3D &tex)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return texture[width * yy + xx];
            texture[(height - yy - 1) * width + xx] = tex;
        }

        Vector3D &get_texture(const int &xx, const int &yy)
        {
            if (xx > width || xx < 0 || yy > height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return depth[width * yy + xx];
            return texture[(height - yy - 1) * width + xx];
        }

        void resize(const int &xx, const int &yy)
        {
            width = (std::max(xx, 1), std::max(yy, 1));
            height = std::min(std::max(xx, 1), std::max(yy, 1));
            depth.resize(width * height);
            clear_depth();

            pixel.resize(width * height);
            clear_pixel();

            texture.resize(width * height);
            clear_texture();
        }

        void clear_pixel()
        {
            std::fill(pixel.begin(), pixel.end(), Vector3D(0));
        }

        void clear_depth()
        {
            std::fill(depth.begin(), depth.end(), std::numeric_limits<Number>::min());
        }

        void clear_texture()
        {
            std::fill(texture.begin(), texture.end(), Vector3D(0));
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

        void print_texture()
        {
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    std::cout << get_texture(i, j) << " ";
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
            // 只画线框2D三角形
            draw_line(a, b, color);
            draw_line(b, c, color);
            draw_line(c, a, color);
        }

        void draw_triangle_2D_filled(const Vector2I &a, const Vector2I &b, const Vector2I &c, const Vector3D &color)
        {
            return draw_triangle_2D_filled(Triangle2I(a, b, c), color);
        }

        void draw_triangle_3D_filled(const Triangle3D &tri, const Vector3D &color)
        {
            // 画一个涂满颜色的3D三角形
            // 包围盒
            std::vector<Number> max_vec = tri.maximum();
            std::vector<Number> min_vec = tri.minimum();
            int x_max = std::min((int)max_vec[0], height - 1);
            int y_max = std::min((int)max_vec[1], height - 1);
            int x_min = std::max((int)min_vec[0], 0);
            int y_min = std::max((int)min_vec[1], 0);
            // 遍历包围盒
            Vector3D p;
            for (p.x = x_min; p.x <= x_max; p.x++)
            {
                for (p.y = y_min; p.y <= y_max; p.y++)
                {
                    Vector3D bc_screen = tri.barycentric(p);
                    // 重心在三角形外不渲染
                    if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                    {
                        continue;
                    }
                    p.z = tri.a.z * bc_screen.x + tri.b.z * bc_screen.y + tri.c.z * bc_screen.z;
                    if (get_depth_value(p.x, p.y) < p.z)
                    {
                        set_depth_value(p.x, p.y, p.z);
                        set_pixel_color(p.x, p.y, color);
                    }
                }
            }
        }

        void draw_triangle_3D_uv(const Triangle3D &tri)
        {
            // 画一个显示UV值的3D三角形
            // 包围盒
            std::vector<Number> max_vec = tri.maximum();
            std::vector<Number> min_vec = tri.minimum();
            int x_max = std::min((int)max_vec[0], height - 1);
            int y_max = std::min((int)max_vec[1], height - 1);
            int x_min = std::max((int)min_vec[0], 0);
            int y_min = std::max((int)min_vec[1], 0);
            // 遍历包围盒
            Vector3D p;
            for (p.x = x_min; p.x <= x_max; p.x++)
            {
                for (p.y = y_min; p.y <= y_max; p.y++)
                {
                    Vector3D bc_screen = tri.barycentric(p);
                    // 重心在三角形外不渲染
                    if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                    {
                        continue;
                    }
                    p.z = tri.a.z * bc_screen.x + tri.b.z * bc_screen.y + tri.c.z * bc_screen.z;
                    if (get_depth_value(p.x, p.y) < p.z)
                    {
                        set_depth_value(p.x, p.y, p.z);
                        Number color_x = tri.a_color_pos.x * bc_screen.x + tri.b_color_pos.x * bc_screen.y + tri.c_color_pos.x * bc_screen.z;
                        Number color_y = tri.a_color_pos.y * bc_screen.x + tri.b_color_pos.y * bc_screen.y + tri.c_color_pos.y * bc_screen.z;
                        set_pixel_color(p.x, p.y, {color_x * 255.0, color_y * 255.0, 0});
                    }
                }
            }
        }

        void draw_triangle_3D_textured(const Triangle3D &tri, Mesh &mesh)
        {
            // 画一个涂满颜色的3D三角形
            // 包围盒
            std::vector<Number> max_vec = tri.maximum();
            std::vector<Number> min_vec = tri.minimum();
            int x_max = std::min((int)max_vec[0], height - 1);
            int y_max = std::min((int)max_vec[1], height - 1);
            int x_min = std::max((int)min_vec[0], 0);
            int y_min = std::max((int)min_vec[1], 0);
            // 遍历包围盒
            Vector3D p;
            for (p.x = x_min; p.x <= x_max; p.x++)
            {
                for (p.y = y_min; p.y <= y_max; p.y++)
                {
                    Vector3D bc_screen = tri.barycentric(p);
                    // 重心在三角形外不渲染
                    if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                    {
                        continue;
                    }
                    p.z = tri.a.z * bc_screen.x + tri.b.z * bc_screen.y + tri.c.z * bc_screen.z;
                    if (get_depth_value(p.x, p.y) < p.z)
                    {
                        set_depth_value(p.x, p.y, p.z);
                        Number color_x = (tri.a_color_pos.x * bc_screen.x + tri.b_color_pos.x * bc_screen.y + tri.c_color_pos.x * bc_screen.z) * mesh.pic_width;
                        Number color_y = (tri.a_color_pos.y * bc_screen.x + tri.b_color_pos.y * bc_screen.y + tri.c_color_pos.y * bc_screen.z) * mesh.pic_height;
                        int pos_x = color_x - (int)color_x >= 0.5 ? (int)(color_x) + 1 : (int)color_x;
                        int pos_y = color_y - (int)color_y >= 0.5 ? (int)(color_y) + 1 : (int)color_y;
                        Vector3D color = mesh.get_texture_pic_color(pos_x, pos_y);
                        set_pixel_color(p.x, p.y, color);
                    }
                }
            }
        }

        void draw_triangle_3D_gouraud_shaded(const Triangle3D &tri, const Vector3D &light_dir, const Vector3D &color, Mesh &mesh)
        {
            // 画一个涂满颜色的3D三角形
            // 包围盒
            std::vector<Number> max_vec = tri.maximum();
            std::vector<Number> min_vec = tri.minimum();
            int x_max = std::min((int)max_vec[0], height - 1);
            int y_max = std::min((int)max_vec[1], height - 1);
            int x_min = std::max((int)min_vec[0], 0);
            int y_min = std::max((int)min_vec[1], 0);

            bool has_texture = !mesh.pic.empty();

            // 遍历包围盒
            Vector3D p;
            for (p.x = x_min; p.x <= x_max; p.x++)
            {
                for (p.y = y_min; p.y <= y_max; p.y++)
                {
                    Vector3D bc_screen = tri.barycentric(p);
                    // 重心在三角形外不渲染
                    if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                    {
                        continue;
                    }
                    p.z = tri.a.z * bc_screen.x + tri.b.z * bc_screen.y + tri.c.z * bc_screen.z;
                    if (get_depth_value(p.x, p.y) < p.z)
                    {
                        set_depth_value(p.x, p.y, p.z);
                        Number normal_x = (tri.a_normal.x * bc_screen.x + tri.b_normal.x * bc_screen.y + tri.c_normal.x * bc_screen.z);
                        Number normal_y = (tri.a_normal.y * bc_screen.x + tri.b_normal.y * bc_screen.y + tri.c_normal.y * bc_screen.z);
                        Number normal_z = (tri.a_normal.z * bc_screen.x + tri.b_normal.z * bc_screen.y + tri.c_normal.z * bc_screen.z);
                        if (has_texture)
                        {
                            Number color_x = (tri.a_color_pos.x * bc_screen.x + tri.b_color_pos.x * bc_screen.y + tri.c_color_pos.x * bc_screen.z) * mesh.pic_width;
                            Number color_y = (tri.a_color_pos.y * bc_screen.x + tri.b_color_pos.y * bc_screen.y + tri.c_color_pos.y * bc_screen.z) * mesh.pic_height;
                            int pos_x = color_x - (int)color_x >= 0.5 ? (int)(color_x) + 1 : (int)color_x;
                            int pos_y = color_y - (int)color_y >= 0.5 ? (int)(color_y) + 1 : (int)color_y;
                            Vector3D color1 = mesh.get_texture_pic_color(pos_x, pos_y);
                            // intensity = normal * light_dir
                            // color = intensity * color
                            Vector3D color2 = color1 * (Vector3D(normal_x, normal_y, normal_z).normalized() * (light_dir.normalized()));
                            set_pixel_color(p.x, p.y, color2);
                        }
                        else
                        {
                            Vector3D color2 = color * (Vector3D(normal_x, normal_y, normal_z).normalized() * (light_dir.normalized()));
                            set_pixel_color(p.x, p.y, color2);
                        }
                    }
                }
            }
        }

        void draw_triangle_2D_filled(const Triangle2I &tri, const Vector3D &color)
        {
            // 画一个涂满颜色的2D三角形
            // 包围盒
            std::vector<int> max_vec = tri.maximum();
            std::vector<int> min_vec = tri.minimum();
            int x_max = std::min((int)max_vec[0], height - 1);
            int y_max = std::min((int)max_vec[1], height - 1);
            int x_min = std::max((int)min_vec[0], 0);
            int y_min = std::max((int)min_vec[1], 0);

            // 遍历包围盒
            Vector2I p;
            for (p.x = x_min; p.x <= x_max; p.x++)
            {
                for (p.y = y_min; p.y <= y_max; p.y++)
                {
                    Vector3D bc_screen = tri.barycentric(p);
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
                draw_triangle_2D_filled(tri, color);
            }
        }

        void set_mesh_filled_gray(Mesh &mesh, const Vector3D &light_dir)
        {
            // 将模型的每个面都涂上随机颜色
            for (int i = 0; i < mesh.face_num(); i++)
            {
                std::vector<int> face = mesh.faces[i];
                Vector3D screen_coords[3];
                Vector3D world_coords[3];
                for (int j = 0; j < 3; j++)
                {
                    world_coords[j] = mesh.vertices[face[j]];
                    screen_coords[j] = world_to_screen(world_coords[j]);
                }
                Triangle3D tri(screen_coords[0], screen_coords[1], screen_coords[2]);
                Vector3D normal = Triangle3D(world_coords[0], world_coords[1], world_coords[2]).normal;
                Number intensity = normal * light_dir;
                Vector3D color(intensity * 255, intensity * 255, intensity * 255);
                draw_triangle_3D_filled(tri, color);
            }
        }

        void set_mesh_with_uv(Mesh &mesh)
        {
            // 将模型的每个面都涂上随机颜色
            for (int i = 0; i < mesh.face_num(); i++)
            {
                std::vector<int> face = mesh.faces[i];
                Vector3D screen_coords[3];
                Vector3D world_coords[3];
                Vector3D uvw[3];
                for (int j = 0; j < 3; j++)
                {
                    world_coords[j] = mesh.vertices[face[j]];
                    screen_coords[j] = world_to_screen(world_coords[j]);
                    uvw[j] = mesh.texture[mesh.face_tex[i][j]];
                }
                Triangle3D tri(screen_coords[0], screen_coords[1], screen_coords[2], uvw[0], uvw[1], uvw[2]);
                draw_triangle_3D_uv(tri);
            }
        }

        void set_mesh_textured(Mesh &mesh)
        {
            bool has_texture = !mesh.pic.empty();
            // 将模型的每个面都涂上Texture的颜色
            for (int i = 0; i < mesh.face_num(); i++)
            {
                std::vector<int> face = mesh.faces[i];
                Vector3D screen_coords[3];
                Vector3D world_coords[3];
                Vector3D color_pos[3];

                for (int j = 0; j < 3; j++)
                {
                    world_coords[j] = mesh.vertices[face[j]];
                    screen_coords[j] = world_to_screen(world_coords[j]);
                    if (has_texture)
                    {
                        color_pos[j] = mesh.texture[mesh.face_tex[i][j]];
                    }
                }
                if (has_texture)
                {
                    Triangle3D tri(screen_coords[0], screen_coords[1], screen_coords[2], color_pos[0], color_pos[1], color_pos[2]);
                    draw_triangle_3D_textured(tri, mesh);
                }
                else
                {
                    Triangle3D tri(screen_coords[0], screen_coords[1], screen_coords[2]);
                    Vector3D color(rand() % 255, rand() % 255, rand() % 255);
                    draw_triangle_3D_filled(tri, color);
                }
            }
        }

        void set_mesh_gouraud_shaded(Mesh &mesh, const Matrix4D4 &view, const Matrix4D4 &proj, const Matrix4D4 &mod, const Vector3D &light_dir, const Vector3D &color)
        {
            for (int i = 0; i < mesh.face_num(); i++)
            {
                std::vector<int> face = mesh.faces[i];
                Vector3D screen_coords[3];
                Vector3D world_coords[3];
                Vector3D color_pos[3];
                Vector3D normals[3];

                for (int j = 0; j < 3; j++)
                {
                    Vector3D v = mesh.vertices[face[j]];
                    screen_coords[j] = Vector3D(view * proj * mod * Matrix4D1(v));
                    world_coords[j] = v;
                    color_pos[j] = mesh.texture[mesh.face_tex[i][j]];
                    normals[j] = mesh.norms[face[j]];
                }

                Triangle3D tri(screen_coords[0], screen_coords[1], screen_coords[2], color_pos[0], color_pos[1], color_pos[2], normals[0], normals[1], normals[2]);
                draw_triangle_3D_gouraud_shaded(tri, light_dir, color, mesh);
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
                Number intensity = normal * light_dir;
                if (intensity > 0)
                {
                    // 默认光照是白色
                    Vector3D color(intensity * 255, intensity * 255, intensity * 255);
                    draw_triangle_2D_filled(tri, color);
                }
            }
        }

        Vector3D world_to_screen(const Vector3D &world)
        {
            return Vector3D((world.x + 1.) * width / 2. + 0.5, (world.y + 1.) * height / 2. + 0.5, world.z);
        }
    };
}