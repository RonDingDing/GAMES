#pragma once
#include "Buffers.hpp"
#include "VectorN.hpp"
#include "TgaImage.hpp"
#include <iostream>
#include <vector>
#include <fstream>

namespace Rasterizer
{
    class PpmImage
    {
    public:
        int width = 0;
        int height = 0;
        std::vector<Vector3D> buffer = {};

        PpmImage(const Buffers &buf) : width(buf.width), height(buf.height), buffer(buf.pixel)
        {
        }

        PpmImage(const TgaImage &tga) : width(tga.width), height(tga.height), buffer(tga.buffer)
        {
            // std::cout << tga.buffer.size() <<"   "<< tga.width * tga.height << std::endl;
        }

        void draw_to(const std::string &filename)
        {
            std::ofstream ofs; // 把帧缓存保存到文件中
            std::string filename2 = std::string("./") + filename + std::string(".ppm");
            ofs.open(filename2);
            ofs << "P6\n"
                << width << " " << height << "\n255\n";
            for (int i = 0; i < height * width; ++i)
            {
                ofs << (char)(std::max(0.0, std::min(255.0, buffer[i].x)));
                ofs << (char)(std::max(0.0, std::min(255.0, buffer[i].y)));
                ofs << (char)(std::max(0.0, std::min(255.0, buffer[i].z)));
            }
            ofs.close();
        }
    };
}