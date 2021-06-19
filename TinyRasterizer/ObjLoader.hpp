#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "VectorN.hpp"
#include "Mesh.hpp"

namespace Rasterizer
{
    using Number = double;
    class ObjectLoader
    {
    public:
        std::vector<Vector3D> vertices;
        std::vector<std::vector<int>> faces;
        std::vector<Vector3D> texture;
        std::vector<std::vector<int>> face_tex;

        void clear()
        {
            vertices.clear();
            faces.clear();
            texture.clear();
            face_tex.clear();
        }

        Mesh load(const char *filename)
        {
            clear();
            std::ifstream obj_file(filename);
            if (!obj_file.is_open())
            {
                std::cout << "Cannot open this file: " << filename << std::endl;
                return Mesh();
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
                else if (!line.compare(0, 2, "f "))
                {
                    load_faces(line_stream);
                }
                else if (!line.compare(0, 3, "vt "))
                {
                    load_texture(line_stream);
                }
            }
            std::cout << "verticess num: " << vertices.size() << ", face num: " << faces.size() << ", texture num: " << texture.size() << std::endl;
            return Mesh(vertices, faces);
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

                one_face_tex.emplace_back(t1 - 1);
                one_face_tex.emplace_back(t2 - 1);
                one_face_tex.emplace_back(t3 - 1);
                texture.emplace_back(one_face_tex);
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