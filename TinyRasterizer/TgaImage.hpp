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
            std::ifstream tga_file(filename, std::ios::in | std::ios::binary);
            if (!tga_file.is_open())
            {
                std::cout << "Cannot open this file: " << filename << std::endl;
                return;
            }

            int file_size = read_size(tga_file);
            int version = read_version(tga_file);
            TgaHeader header;
            read_header(tga_file, header);
            std::cout << "File size: " << file_size << std::endl;
            std::cout << "TGA version: " << version << std::endl;
            std::cout << "ID LENGTH: " << (int)header.id_length << std::endl;
            std::cout << "CM TYPE: " << (int)header.color_map_type << std::endl;
            std::cout << "IMAGE TYPE: " << (int)header.image_type << std::endl;
            std::cout << "FIRST MAP ENTRY: " << header.color_map_start << std::endl;
            std::cout << "NUM MAP ENTRIES: " << header.color_map_length << std::endl;
            std::cout << "BYTES PER ENTRY: " << (int)header.color_map_depth << std::endl;
            std::cout << "ORIGIN: " << header.x_offset << "," << header.x_offset << std::endl;
            std::cout << "SIZE: " << header.width << "," << header.height << ":" << (int)header.pixel_depth << std::endl;
            std::cout << "DESCRIPTOR: " << (int)header.image_descriptor << std::endl;

            width = header.width;
            height = header.height;

            // TODO
        }

    private:
        int read_size(std::ifstream &tga_file)
        {
            tga_file.seekg(0, std::ios::end);
            int file_size = (int)tga_file.tellg();

            return file_size;
        }

        int read_version(std::ifstream &tga_file)
        {
            char version_string[17] = "";
            tga_file.seekg(-18, std::ios::end);
            tga_file.read(version_string, 16);
            int version = (std::string(version_string) == "TRUEVISION-XFILE") ? 2 : 1;
            tga_file.seekg(0, std::ios::beg);
            return version;
        }

        void read_header(std::ifstream &tga_file, TgaHeader &header)
        {
            tga_file.read((char *)(&header.id_length), sizeof(header.id_length));
            tga_file.read((char *)(&header.color_map_type), sizeof(header.color_map_type));
            tga_file.read((char *)(&header.image_type), sizeof(header.image_type));
            tga_file.read((char *)(&header.color_map_start), sizeof(header.color_map_start));
            tga_file.read((char *)(&header.color_map_length), sizeof(header.color_map_length));
            tga_file.read((char *)(&header.color_map_depth), sizeof(header.color_map_depth));
            tga_file.read((char *)(&header.x_offset), sizeof(header.x_offset));
            tga_file.read((char *)(&header.y_offset), sizeof(header.y_offset));
            tga_file.read((char *)(&header.width), sizeof(header.width));
            tga_file.read((char *)(&header.height), sizeof(header.height));
            tga_file.read((char *)(&header.pixel_depth), sizeof(header.pixel_depth));
            tga_file.read((char *)(&header.image_descriptor), sizeof(header.image_descriptor));
            return;
        }
    };
}