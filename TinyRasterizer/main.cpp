#include <iostream>
#include <vector>

#include "VectorN.hpp"
#include "Buffers.hpp"
#include "PpmImage.hpp"
#include "TgaImage.hpp"
#include "ObjLoader.hpp"

using namespace Rasterizer;

Vector3D red = {0xff, 0x0, 0x0};
Vector3D orange = {0xff, 0x7f, 0x0};
Vector3D yellow = {0xff, 0xff, 0x0};
Vector3D green = {0x0, 0xff, 0x0};
Vector3D blue = {0x0, 0x0, 0xff};
Vector3D indigo = {0x0, 0xff, 0xff};
Vector3D purple = {0x8b, 0x0, 0xff};
Vector3D white = {0xff, 0xff, 0xff};

void print_color(const Vector3D &color)
{
    Buffers buf = {700, 700};
    for (int i = 0; i < 700 * 100; i++)
    {
        buf.pixel[i] = color;
    }
     
    PpmImage c = {buf};
    c.draw_to("output");
}

int main(int argc, char **argv)
{
    Buffers buf = {1000, 1000};
    // buf.draw_line(Vector2D(1, 11), Vector2D(100, 800), red);
    // buf.draw_line(Vector2D(100, 800), Vector2D(500, 300), green);
    // buf.draw_line(Vector2D(500, 300), Vector2D(1, 11), white);

    // PicConvert con = {buf};
    // con.draw_ppm_to("output");
    // ObjectLoader loader;
    // Mesh mesh = loader.load("head.obj");
    // buf.set_mesh_filled_gray(mesh, Vector3D(0, 0, 1));
    // buf.set_mesh_with_uv(mesh);
    // buf.set_mesh_gray(mesh, Vector3D(0, 0, -1));
    // std::cout << buf.pixel.size() << std::endl;

    // buf.draw_line(Vector2I(20, 34), Vector2I(744, 400), red);
    // buf.draw_line(Vector2I(120, 434), Vector2I(444, 400), green);
    // buf.draw_line(Vector2I(330, 463), Vector2I(594, 200), blue);

    TgaImage tgaimage;
    tgaimage.load("head.tga");
    // // screen line
    // buf.draw_line(Vector2I(10, 10), Vector2I(790, 10), white);
    // PpmImage con = PpmImage(buf);
    // con.draw_to("output");
}