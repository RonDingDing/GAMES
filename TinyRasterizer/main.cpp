#include <iostream>
#include <vector>

#include "VectorN.hpp"
#include "Buffers.hpp"
#include "PicConvert.hpp"
#include "ObjLoader.hpp"

using namespace Rasterizer;

void print_color(const Vector3D &vector)
{
    Buffers buf = {800, 600};
    for (int i = 0; i < 800 * 600; i++)
    {
        buf.pixel[i] = vector;
    }

    PicConvert c = {buf};
    c.draw_ppm_to("output");
}

int main(int argc, char **argv)
{
    // Buffers buf = {800, 600};
    // Vector3D white = {254, 254, 254};
    // Vector3D green = {0, 254, 0};
    // Vector3D red = {254, 0, 0};

    // buf.draw_line(Vector2D(20, 13), Vector2D(40, 80), red);
    // buf.draw_line(Vector2D(80, 40), Vector2D(13, 20), green);
    // buf.draw_line(Vector2D(13, 20), Vector2D(80, 40), white);

    // PicConvert con = {buf};
    // con.draw_ppm_to("output");
    // print_color({255, 255, 255});
    ObjectLoader loader;
    Mesh mesh = loader.load("head.obj");
    Buffers buf = Buffers(1000, 1000);
    buf.set_mesh(mesh, Vector3D(255, 255, 255));
    std::cout << buf.pixel.size() << std::endl;
    PicConvert con = PicConvert(buf);
    con.draw_ppm_to("output");
}