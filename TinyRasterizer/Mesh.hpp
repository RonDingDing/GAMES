#pragma once
#include <vector>
#include "VectorN.hpp"

namespace Rasterizer
{
    class Mesh
    {
    public:
        std::vector<Vector3D> vertices, texture, pic, norms;
        std::vector<std::vector<int>> faces, face_tex;
        int pic_width, pic_height;

        Mesh() : vertices(), texture(), pic(), norms(), faces(), face_tex(), pic_width(0), pic_height(0) {}
        Mesh(std::vector<Vector3D> &vs, std::vector<Vector3D> &tex, std::vector<Vector3D> &pi, std::vector<Vector3D> &ns, std::vector<std::vector<int>> &fs, std::vector<std::vector<int>> &f_t, const int &wid, const int &hei)
            : vertices(vs), texture(tex), pic(pi), norms(ns), faces(fs), face_tex(f_t), pic_width(wid), pic_height(hei)
        {
        }
        Mesh(std::vector<Vector3D> &vs, std::vector<Vector3D> &tex, std::vector<Vector3D> &ns, std::vector<std::vector<int>> &fs, std::vector<std::vector<int>> &f_t)
            : vertices(vs), texture(tex), pic(), norms(ns), faces(fs), face_tex(f_t), pic_width(), pic_height()
        {
        }
        int vert_num() { return (int)vertices.size(); }
        int face_num() { return (int)faces.size(); }
        int face_tex_num() { return (int)face_tex.size(); }
        int texture_num() { return (int)texture.size(); }

        Vector3D &get_texture_pic_color(int xx, int yy)
        {
            // if (xx > pic_width)
            // {
            //     xx = pic_width;
            // }
            // if (xx < 0)
            // {
            //     xx = 0;
            // }
            // if (yy > pic_height)
            // {
            //     yy = pic_height;
            // }
            // if (yy < 0)
            // {
            //     yy = 0;
            // }
            if (xx > pic_width || xx < 0 || yy > pic_height || yy < 0)
            {
                throw std::out_of_range("Out of buffer range!");
            }
            // return pixel[width * yy + xx];
            return pic[(pic_height - yy - 1) * pic_width + xx];
        }
    };
}