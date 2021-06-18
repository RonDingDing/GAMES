#pragma once
#include <vector>
#include "VectorN.hpp"

namespace Rasterizer
{
    class Mesh
    {
    public:
        std::vector<Vector3D> vertices;
        std::vector<std::vector<int>> faces;
        Mesh() : vertices(), faces() {}
        Mesh(std::vector<Vector3D> &vs, std::vector<std::vector<int>> &fs) : vertices(vs), faces(fs) {}
        int vert_num() { return (int)vertices.size(); }
        int face_num() { return (int)faces.size(); }
    };
}