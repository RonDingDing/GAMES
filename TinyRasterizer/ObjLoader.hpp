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
    class ObjectLoader
    {
    public:
        std::vector<Vector3D> vertices;
        std::vector<std::vector<int>> faces;

        Mesh load(const char *filename)
        {
            std::ifstream obj_file(filename);
            if (!obj_file.is_open())
            {
                std::cout << "Cannot open this file: " << filename << std::endl;
                return Mesh( );
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
            }
            std::cout << "# v# " << vertices.size() << " f# " << faces.size() << std::endl;
            return Mesh(vertices, faces);
        }

    private:
        void load_faces(std::istringstream &line_stream)
        {

            std::vector<int> f;
            int idx;
            char ctrash = 0;
            std::string subline1, subline2, subline3;
            line_stream >> ctrash >> subline1 >> subline2 >> subline3;
            if (subline1.find_first_of('/') == std::string::npos) // P
            {
                f.push_back(std::stoi(subline1) - 1);
                f.push_back(std::stoi(subline2) - 1);
                f.push_back(std::stoi(subline3) - 1);
                faces.push_back(f);
            }
            else if (subline1.find("//") != std::string::npos)
            {
                std::istringstream sub1(subline1.c_str());
                std::istringstream sub2(subline2.c_str());
                std::istringstream sub3(subline3.c_str());

                char c2trash = 0;
                int i2trash = 0;

                sub1 >> idx >> ctrash >> c2trash >> i2trash;
                f.push_back(idx - 1);
                sub2 >> idx >> ctrash >> c2trash >> i2trash;
                f.push_back(idx - 1);
                sub3 >> idx >> ctrash >> c2trash >> i2trash;
                f.push_back(idx - 1);

                faces.push_back(f);
            }
            else // P/T/N or P/T
            {
                std::istringstream sub1(subline1.c_str());
                std::istringstream sub2(subline2.c_str());
                std::istringstream sub3(subline3.c_str());

                char c2trash = 0;
                int i2trash = 0;
                int itrash = 0;

                sub1 >> idx >> ctrash >> itrash >> c2trash >> i2trash;
                f.push_back(idx - 1);
                sub2 >> idx >> ctrash >> itrash >> c2trash >> i2trash;
                f.push_back(idx - 1);
                sub3 >> idx >> ctrash >> itrash >> c2trash >> i2trash;
                f.push_back(idx - 1);

                faces.push_back(f);
            }
        }

        void load_vertices(std::istringstream &line_stream)
        {

            char ctrash = 0;
            line_stream >> ctrash;
            Vector3D v;
            line_stream >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
    };
}