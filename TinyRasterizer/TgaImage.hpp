#pragma once
#include "Buffers.hpp"
#include "VectorN.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <stdio.h>

void show_bytes(unsigned char *start, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

bool is_little_endian()
{
    int i = 0x11223344;
    char *p;

    p = (char *)&i;
    if (*p == 0x44)
    {
        return true;
    }
    return false;
}

template <typename Type>
Type read(std::ifstream &tga_file, size_t num)
{
    char some[num];
    tga_file.read(some, num);

    show_bytes((unsigned char *)(&some), num);
    // if (!is_little_endian())
    // {
    //     for (size_t i = 0; i < num - 1; i++)
    //     {
    //         char s = some[i];
    //         some[i] = some[num - i - 1];
    //         some[num - i - 1] = s;
    //     }
    // }

    Type *a = (Type *)(some);
    return *a;
}

namespace Rasterizer
{
    using Byte = unsigned char;
    using Word = unsigned short;
    struct TgaHeader
    {
        Byte id_length;        /* 00h  Size of Image ID field */
        Byte color_map_type;   /* 01h  Color map type */
        Byte image_type;       /* 02h  Image type code */
        Word color_map_start;  /* 03h  Color map origin */
        Word color_map_length; /* 05h  Color map length */
        Byte color_map_depth;  /* 07h  Depth of color map entries */
        Word x_offset;         /* 08h  X origin of image */
        Word y_offset;         /* 0Ah  Y origin of image */
        Word width;            /* 0Ch  Width of image */
        Word height;           /* 0Eh  Height of image */
        Byte pixel_depth;      /* 10h  Image pixel size */
        Byte image_descriptor; /* 11h  Image descriptor byte */
    };

    class TgaImage
    {
    public:
        char *data;
        int width = 0;
        int height = 0;
        int bytespp = 0;

        std::vector<Vector3D> buffer = {};
        TgaImage() : data(nullptr), width(0), height(0), bytespp(0) {}
        TgaImage(const Buffers &buf) : width(buf.width), height(buf.height), buffer(buf.pixel)
        {
        }

        void draw_to(const std::string &filename)
        {
        }

        void load(const char *filename)
        {
            if (data)
            {
                delete[] data;
            }
            data = nullptr;
            std::ifstream tga_file(filename);
            tga_file.open(filename, std::ios::binary);
            if (!tga_file.is_open())
            {
                std::cout << "Cannot open this file: " << filename << std::endl;
                tga_file.close();
                return;
            }
            read<int>(tga_file, 4);
            // TODO
        }
    };
}