#include "VectorN.hpp"
#include <iostream>
int main(int argc, char **argv)
{
    Rasterizer::Vector3D v1 = {1, 0, 0};
    Rasterizer::Vector3D v2 = {0, 1, 0};
    std::cout << v1 + v2 << std::endl;
    std::cout << v1 - v2 << std::endl;
    std::cout << v1 * v2 << std::endl;

    std::cout << v1.dot(v2) << std::endl;
    std::cout << v1.cross(v2) << std::endl;
    std::cout << v1.len() << std::endl;
    std::cout << v1.normalized() << std::endl;
    std::cout << v1.lerp(v2, 0.22) << std::endl;
}