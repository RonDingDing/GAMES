#pragma once
#include <vector>
#include "VectorN.hpp"

namespace Rasterizer
{
    class Mesh
    {
    public:
        std::vector<Vector3D> vertices, texture, pic;
        std::vector<std::vector<int>> faces, face_tex;
        int width, height;

        Mesh() : vertices(), texture(), faces(), face_tex() {}
        Mesh(std::vector<Vector3D> &vs, std::vector<Vector3D> &tex, std::vector<Vector3D> &pi, std::vector<std::vector<int>> &fs, std::vector<std::vector<int>> &f_t, const int &wid, const int &hei)
            : vertices(vs), texture(tex), pic(pi), faces(fs), face_tex(f_t), width(wid), height(hei)
        {
        }
        Mesh(std::vector<Vector3D> &vs, std::vector<Vector3D> &tex, std::vector<std::vector<int>> &fs, std::vector<std::vector<int>> &f_t)
            : vertices(vs), texture(tex), pic(), faces(fs), face_tex(f_t), width(), height()
        {
        }
        int vert_num() { return (int)vertices.size(); }
        int face_num() { return (int)faces.size(); }
        int face_tex_num() { return (int)face_tex.size(); }
        int texture_num() { return (int)texture.size(); }
    };
}