#include <iostream>
#include <vector>
#include <optional>

#include "VectorN.hpp"
#include "Buffers.hpp"
#include "PpmImage.hpp"
#include "TgaImage.hpp"
#include "ObjLoader.hpp"
#include "Transformation.hpp"

using namespace Rasterizer;

Vector3D red = {0xff, 0x0, 0x0};
Vector3D orange = {0xff, 0x7f, 0x0};
Vector3D yellow = {0xff, 0xff, 0x0};
Vector3D green = {0x0, 0xff, 0x0};
Vector3D blue = {0x0, 0x0, 0xff};
Vector3D indigo = {0x0, 0xff, 0xff};
Vector3D purple = {0x8b, 0x0, 0xff};
Vector3D white = {0xff, 0xff, 0xff};

// void print_color(color)
// {
//     Buffers buf = {700, 700};
//     for (int i = 0; i < 700 * 100; i++)
//     {
//         buf.pixel[i] = color;
//     }

//     PpmImage c = {buf};
//     c.draw_to("output");
// }

int main(int argc, char **argv)
{
    const int width = 800;
    const int height = 800;
    const int depth = 255;
    Buffers buf = {width, height};
    // buf.draw_line(Vector2D(1, 11), Vector2D(100, 800), red);
    // buf.draw_line(Vector2D(100, 800), Vector2D(500, 300), green);
    // buf.draw_line(Vector2D(500, 300), Vector2D(1, 11), white);

    // PicConvert con = {buf};
    // con.draw_ppm_to("output");
    ObjectLoader loader;
    std::optional<Mesh> mesh_p = loader.load("head");
    if (mesh_p)
    {
        const Vector3D eye(1, 1, 3);
        const Vector3D center(0, 0, 0);
        const Vector3D up(0, 1, 0);
        const Vector3D light_dir(1, -1, 1);
        const int x = width / 8, y = height / 8, w = width * 3 / 4, h = height * 3 / 4;
        Mesh mesh = *mesh_p;
        // buf.set_mesh_textured(mesh);

        Matrix4D4 transformation = Transformation::get_matrix(x, y, w, h, depth, eye, center, up);
        buf.set_mesh_grouraud_shaded(mesh, transformation, light_dir);
        // buf.set_mesh_filled_gray(mesh, light_dir);
        PpmImage con = PpmImage(buf);
        con.draw_to("output");
    }
    // buf.set_mesh_filled_gray(mesh, Vector3D(0, 0, 1));
    // buf.set_mesh_with_uv(mesh);
    // buf.set_mesh_gray(mesh, Vector3D(0, 0, -1));
    // std::cout << buf.pixel.size() << std::endl;

    // buf.draw_line(Vector2I(20, 34), Vector2I(744, 400), red);
    // buf.draw_line(Vector2I(120, 434), Vector2I(444, 400), green);
    // buf.draw_line(Vector2I(330, 463), Vector2I(594, 200), blue);
    // std::string name;
    // std::cout << "Enter file name:";
    // std::cin >> name;

    // if (tgaimage.load("head.tga"))
    // {

    //     // tgaimage.print_pixel();
    //     // tgaimage.flip_x();
    //     // std::cout << std::endl;
    //     // tgaimage.print_pixel();

    //     // }

    //     // // screen line
    //     // buf.draw_line(Vector2I(10, 10), Vector2I(790, 10), white);
    //     PpmImage con = PpmImage(tgaimage);
    //     con.draw_to("output");
    // }

    // Matrix4D a = Matrix4D();
    // a.reset_pointer();
    // a << 1.0 << 2.0 << 3.0 << 4.0
    //   << 5.0 << 6.0 << 7.0 << 8.0
    //   << 9.0 << 10.0 << 11.0 << 12.0
    //   << 13.0 << 14.0 << 15.0 << 16.0
    //   << 17.0 << 18.0 << 19.0 << 20.0;
    //  << a << std::endl;
}

// int main(int argc, char **argv)
// {
//     Vector3D a = {4, 5, 6};
//     Matrix4D1 b = {a};
//     Vector3D c = {b};
//     std::cout << b << std::endl;
//     std::cout << c << std::endl;
// }