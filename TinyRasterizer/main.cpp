#include <iostream>
#include <vector>

#include "VectorN.hpp"
#include "Buffers.hpp"
#include "PicConvert.hpp"

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
    print_color({122, 221, 23});
}