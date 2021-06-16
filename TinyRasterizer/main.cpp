#include "VectorN.hpp"
#include "Buffers.hpp"
#include <iostream>
#include <vector>
int main(int argc, char **argv)
{
    // Vector3D t = {1, 1, 1};
    // Vector3D v = {1, 1, 1};

    // std::cout << t + v << std::endl;
    // std::cout << t - v << std::endl;
    // std::cout << t * v << std::endl;
    // std::cout << t * 2 << std::endl;
    // std::cout << t / 3 << std::endl;
    // std::cout << t.dot(v) << std::endl;
    // std::cout << t.cross(v) << std::endl;
    // std::cout << t.len() << std::endl;
    // std::cout << t.len_square() << std::endl;
    // std::cout << t[0] << std::endl;
    // std::cout << t[1] << std::endl;
    // std::cout << t[2] << std::endl;
    using namespace Rasterizer;

    Buffers buf = {4, 3};
    buf.set_pixel_color(0, 0, Vector3D(1));

    buf.set_pixel_color(0, 1, Vector3D(2));
    buf.set_pixel_color(0, 2, Vector3D(3));
    buf.set_pixel_color(1, 0, Vector3D(5));
    buf.set_pixel_color(1, 0, Vector3D(5));
    buf.set_pixel_color(1, 1, Vector3D(6));
    buf.set_pixel_color(1, 2, Vector3D(7));
    buf.set_pixel_color(2, 0, Vector3D(8));
    buf.set_pixel_color(2, 1, Vector3D(9));
    buf.set_pixel_color(2, 2, Vector3D(10));
    buf.set_pixel_color(3, 0, Vector3D(11));
    buf.set_pixel_color(3, 1, Vector3D(12));
    buf.set_pixel_color(3, 2, Vector3D(13));

    double d = buf.get_depth_value(2, 1);
    d  = 2;

    std::cout << d << std::endl
              << std::endl;

    buf.print_pixel();
    std::cout << std::endl;
    buf.print_depth();
}