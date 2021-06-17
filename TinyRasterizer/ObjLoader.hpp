#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "VectorN.hpp"

namespace Rasterizer
{
    class ObjectLoader
    {
    public:
        std::vector<Vector3D> vertices;
        std::vector<std::vector<int>> faces;

        void load(const char *filename)
        {
            std::ifstream obj_file(filename);
            if (!obj_file.is_open())
            {
                std::cout << "Cannot open this file: " << filename << std::endl;
                return;
            }

            std::string line;
            while (getline(obj_file, line))
            {
                std::vector<std::string> list = strip_and_split(line);
                if (list[0] == std::string("v"))
                {
                    load_vertices(list);
                }
                else
                    std::cout << list[0];
            }
        }

    private:
        void load_vertices(const std::vector<std::string> &list)
        {
            if (list.size() != 4 && list.size() != 5)
            {
                throw std::out_of_range("Load vertices error.");
            }
            int num = 0;
            Vector3D vec;
            for (auto string_piece : list)
            {
                if (string_piece != "v")
                {
                    switch (num)
                    {
                    case 0:
                        vec.x = std::stod(string_piece);
                        num++;
                        break;
                    case 1:
                        vec.y = std::stod(string_piece);
                        num++;
                        break;
                    case 2:
                        vec.z = std::stod(string_piece);
                        num++;
                        break;
                    }
                }
            }
            vertices.push_back(vec);
        }

        std::vector<std::string> strip_and_split(std::string &str)
        {
            std::vector<std::string> list;
            size_t s = 0;
            for (size_t i = 0; i < str.size(); i++)
            {
                if (str[i] == '\t' || str[i] == ' ' || i == str.size() - 1)
                {
                    list.push_back(str.substr(s, i - s));
                    s = i + 1;
                }
            }
            return list;
        }
    };
}