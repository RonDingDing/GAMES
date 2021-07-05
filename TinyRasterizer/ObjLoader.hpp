#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include "VectorN.hpp"
#include "Mesh.hpp"

namespace Rasterizer
{
    using Number = double;
    class ObjectLoader
    {
    public:
        std::vector<Vector3D> vertices, texture;
        std::vector<std::vector<int>> faces, face_tex;
        TgaImage texture_pic;

        void clear()
        {
            vertices.clear();
            texture.clear();
            faces.clear();
            face_tex.clear();
        }

        std::optional<Mesh> load(const std::string &file_basename)
        {
            clear();
            std::string obj_filename = file_basename + ".obj";
            std::ifstream obj_file(obj_filename);
            if (!obj_file.is_open())
            {
                std::cout << "Cannot open this file: " << obj_filename << std::endl;
                return std::nullopt;
            }
            std::string line;
            while (!obj_file.eof())
            {
                std::getline(obj_file, line);
                std::istringstream line_stream(line.c_str());

                if (!line.compare(0, 2, "v "))
                {
                    load_vertices(line_stream);
                }
                else if (!line.compare(0, 3, "vn "))
                {
                    load_normal(line_stream);
                }
                else if (!line.compare(0, 2, "f "))
                {
                    load_faces(line_stream);
                }
                else if (!line.compare(0, 3, "vt "))
                {
                    load_texture(line_stream);
                }
            }
#if DEBUG == 1
            std::cout << "verticess num: " << vertices.size() << ", face num: " << faces.size() << ", texture num: " << texture.size() << ", face_tex num: " << face_tex.size() << std::endl;
#endif
            std::string tga_filename = file_basename + ".tga";
            std::optional<TgaImage> has_texture = texture_pic.load(tga_filename);
            if (has_texture)
            {
                return Mesh(vertices, texture, has_texture->buffer, faces, face_tex, has_texture->width, has_texture->height);
            }
            else
            {
                return Mesh(vertices, texture, faces, face_tex);
            }
        }

    private:
        void load_texture(std::istringstream &line_stream)
        {
            char ctrash = 0;
            Number u = 0, v = 0, w = 0;
            while (line_stream >> ctrash >> ctrash >> u >> v >> w)
            {
                texture.emplace_back(u, v, w);
            }
        }

        void load_faces(std::istringstream &line_stream)
        {
            std::vector<int> one_face, one_face_tex;
            int p1 = 0, p2 = 0, p3 = 0;
            char ctrash = 0;
            std::string subline1, subline2, subline3;
            line_stream >> ctrash >> subline1 >> subline2 >> subline3;
            if (subline1.find_first_of('/') == std::string::npos) // P
            {
                one_face.emplace_back(std::stoi(subline1) - 1);
                one_face.emplace_back(std::stoi(subline2) - 1);
                one_face.emplace_back(std::stoi(subline3) - 1);
                faces.emplace_back(one_face);
            }
            else if (subline1.find("//") != std::string::npos)
            {
                std::istringstream sub1(subline1.c_str());
                std::istringstream sub2(subline2.c_str());
                std::istringstream sub3(subline3.c_str());

                char c2trash = 0;
                int n1 = 0, n2 = 0, n3 = 0;

                sub1 >> p1 >> ctrash >> c2trash >> n1;
                sub2 >> p2 >> ctrash >> c2trash >> n2;
                sub3 >> p3 >> ctrash >> c2trash >> n3;
                one_face.emplace_back(p1 - 1);
                one_face.emplace_back(p2 - 1);
                one_face.emplace_back(p3 - 1);

                faces.emplace_back(one_face);
            }
            else // P/T/N or P/T
            {
                std::istringstream sub1(subline1.c_str());
                std::istringstream sub2(subline2.c_str());
                std::istringstream sub3(subline3.c_str());

                char c2trash = 0;
                int n1 = 0, n2 = 0, n3 = 0, t1 = 0, t2 = 0, t3 = 0;

                sub1 >> p1 >> ctrash >> t1 >> c2trash >> n1;
                sub2 >> p2 >> ctrash >> t2 >> c2trash >> n2;
                sub3 >> p3 >> ctrash >> t3 >> c2trash >> n3;
                one_face.emplace_back(p1 - 1);
                one_face.emplace_back(p2 - 1);
                one_face.emplace_back(p3 - 1);
                faces.emplace_back(one_face);

                one_face_tex.emplace_back(t1 - 1);
                one_face_tex.emplace_back(t2 - 1);
                one_face_tex.emplace_back(t3 - 1);
                face_tex.emplace_back(one_face_tex);
            }
        }

        void load_vertices(std::istringstream &line_stream)
        {
            char ctrash = 0;
            line_stream >> ctrash;
            Number x, y, z;

            line_stream >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
    };
}