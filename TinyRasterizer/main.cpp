#include "VectorN.hpp"
#include <iostream>
#include <vector>
int main(int argc, char **argv)
{
    Vector3D t = {1, 1, 1};
    Vector3D v = {1, 1, 1};

    std::cout << t + v << std::endl;
    std::cout << t - v << std::endl;
    std::cout << t * v << std::endl;
    std::cout << t * 2 << std::endl;
    std::cout << t / 3 << std::endl;
    std::cout << t.dot(v) << std::endl;
    std::cout << t.cross(v) << std::endl;
    std::cout << t.len() << std::endl;
    std::cout << t.len_square() << std::endl;
    std::cout << t[0] << std::endl;
    std::cout << t[1] << std::endl;
    std::cout << t[2] << std::endl;
}